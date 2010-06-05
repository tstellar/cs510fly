// Game.cpp
// @author Eric D. Wills

#include "Game.h"

#include "Airplane.h"
#include "ConfigReader.h"
#include "Display.h"
#include "InputListener.h"
#include "Level.h"
#include "World.h"

#ifdef LINUX
    #include <AL/alc.h>
    #include <AL/alure.h>
#else
    #include <OpenAL/alc.h>

    // Yeah, it's deprecated as hell and missing from the headers. But it's still in the binaries. So sue me.
    extern "C" void alutLoadWAVFile(const char*, ALenum*, ALvoid*, ALsizei*, ALsizei*);
    extern "C" void alutUnloadWAV(ALenum, ALvoid*, ALsizei, ALsizei);
#endif

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
// From ExampleApplication.h in Ogre samples

#ifndef LINUX
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif

Game::Game() : currentLevel(NULL), breaking(false) {
#ifndef LINUX       
    mResourcePath = macBundlePath() + "/Contents/Resources/";
    levelPath = mResourcePath;
    audioPath = mResourcePath;
    Ogre::String pluginsPath = macBundlePath() + "/Contents/Resources/plugins.cfg";
    root.reset(new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log"));
#else
    levelPath = "data/levels/";
    audioPath = "data/audio/";
    root.reset(new Ogre::Root("plugins-linux.cfg","ogre.cfg","Ogre.log"));
#endif
}

void Game::init() {
    if (setup()){
        root->startRendering();
    }
}

bool Game::setup() {
    Ogre::ConfigFile cf;

    cf.load(mResourcePath + "resources.cfg");

    // iterate through resources.cfg and register resource groups
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        std::string secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i){
            Ogre::ResourceGroupManager::getSingleton().
                    addResourceLocation(mResourcePath + i->second,
                            i->first, secName);
        }
    }

    // create render window if user completes config dialog
    if (root->showConfigDialog()){
        renderWindow = root->initialise(true);
    }
    else{
        return false;
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // craete scene manager
    sceneManager = root->createSceneManager("DefaultSceneManager");

    Ogre::ColourValue fogColor(0.93f, 0.86f, 0.76f);
    sceneManager->setFog(Ogre::FOG_LINEAR, fogColor, 0.001f, 500.0f, 1000.0f);

    // create camera
    camera = sceneManager->createCamera("PrimaryCamera");
    camera->setNearClipDistance(1.0f);
    camera->setFarClipDistance(10000.0f);

    // create viewport
    Ogre::Viewport* vp = renderWindow->addViewport(camera);
    vp->setBackgroundColour(fogColor);
    camera->setAspectRatio(vp->getActualWidth()/(float)vp->getActualHeight());
    camera->setFOVy(Ogre::Radian(65.0f*Ogre::Math::PI/180.0f));

    // set up lighting
    sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* light = sceneManager->createLight("PrimaryLight");
    light->setPosition(20.0f, 80.0f, 50.0f);

    inputListener.reset(new InputListener(this, renderWindow));
    root->addFrameListener(inputListener.get());
    
    // init sound
    ALCdevice * device = alcOpenDevice(NULL);
    if(device == NULL){
        fprintf(stderr, "Unable to open default audio device.\n");
        return false;
    }
    ALCcontext * context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alDistanceModel(AL_LINEAR_DISTANCE);
    /* Clear error code (I am not sure why this is done, but the example in
     * the OpenAL docs do this.
     */
    alGetError();

    loadWavFile(&motorBuffer, "Running.wav");
    loadWavFile(&enemyBuffer, "R2D2a.wav");

    // create levels
    ConfigReader levelsCfg(getLevelPath() + "levels.cfg");
    const Ogre::StringVector& levelFiles = levelsCfg.parse(
            "Levels", &Ogre::StringConverter::parseStringVector, Ogre::StringVector());
    
    for (Ogre::StringVector::const_iterator iter = levelFiles.begin(); iter != levelFiles.end(); iter++)
        levels.push_back(std::tr1::shared_ptr<Level>(new Level(this, getLevelPath() + *iter)));
    
    startLevel(0);
    
    return true;
}

void Game::startLevel(int index) {
    world.reset(new World(this));
    currentLevel = levels[index].get();
    currentLevel->populate(world.get());

    airplane = world->getPlayer();

    airplane->getSceneNode()->attachObject(camera);
    
    // add skybox
    sceneManager->setSkyBox(true, currentLevel->getSkyBox(), 100);
    
    display.reset(new Display(this));
    display->setup();
}

void Game::update(float dt) {
    world->update(dt);
    display->update(dt);
    const Ogre::Vector3 &cameraPos = camera->getParentNode()->getPosition();
    alListener3f(AL_POSITION, cameraPos.x, cameraPos.y, cameraPos.z);
    /*TODO: Set listener orientation */
}

void Game::setBreak() {
    breaking = true;
}

bool Game::checkBreak() {
    if (breaking) {
        breaking = false;
        return true;
    } else {
        return false;
    }
}

void Game::lose() {
    camera->moveRelative(Ogre::Vector3(0.0f, 10.0f, 50.0f));
    std::cerr << "FAIL\n";
}

bool Game::loadWavFile(ALuint * buffer, std::string file) {

#ifdef LINUX
    *buffer = alureCreateBufferFromFile((audioPath + file).c_str());
#else
    ALenum error;
    alGenBuffers(1, buffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        return false;
    }
    // Load sound file
    ALenum alFormatBuffer;
    char * alBuffer;
    ALsizei alBufferLen;
    ALsizei alFreqBuffer;
    alutLoadWAVFile((audioPath + file).c_str(), &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer);

    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, buffer);
        return false;
    }
    // Copy sound to buffer
    alBufferData(*buffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, buffer);
        return false;
    }
    //Unload sound file
    alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, buffer);
        return false;
    }
#endif
    return true;
}

