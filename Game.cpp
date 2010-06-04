// Game.cpp
// @author Eric D. Wills

#include "Game.h"

#include "Airplane.h"
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

Game::Game() : inputListener(NULL), airplane(NULL),
        currentLevel(NULL), world(NULL), display(NULL), breaking(false) {
#ifndef LINUX       
    mResourcePath = macBundlePath() + "/Contents/Resources/";
    levelPath = mResourcePath;
    Ogre::String pluginsPath = macBundlePath() + "/Contents/Resources/plugins.cfg";
    root = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");
#else
    levelPath = "data/levels/";
    root = new Ogre::Root("plugins-linux.cfg","ogre.cfg","Ogre.log");
#endif
}

Game::~Game() {
    if (inputListener != NULL){
        delete inputListener;
    }
    if (airplane != NULL){
        delete airplane;
    }
    if (currentLevel != NULL){
        delete currentLevel;
    }
    if (display != NULL) {
        delete display;
    }
    delete root;
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

    // load terrain
    //sceneManager->setWorldGeometry("terrain.cfg");
    
    inputListener = new InputListener(this, renderWindow);
    root->addFrameListener(inputListener);
    
    // init sound
    ALenum error;
    ALCdevice * device = alcOpenDevice(NULL);
    if(device == NULL){
        fprintf(stderr, "Unable to open default audio device.\n");
        return false;
    }
    ALCcontext * context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    /* Clear error code (I am not sure why this is done, but the example in
     * the OpenAL docs do this.
     */
    alGetError();
    
    #ifdef LINUX
    motorBuffer = alureCreateBufferFromFile("data/audio/Running.wav");
    #else
/* Aternate way to load a file.  alureCreateBuferFromFile() does all of this,
 * but if we can't use it on OS X, we might have to use the following code.*/
    alGenBuffers(1, &motorBuffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        return false;
    }
    // Load sound file
    ALenum alFormatBuffer;
    char * alBuffer;
    ALsizei alBufferLen;
    ALsizei alFreqBuffer;
    alutLoadWAVFile((macBundlePath() + "/Contents/Resources/Running.wav").c_str(), &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer);

    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, &motorBuffer);
        return false;
    }
    // Copy sound to buffer
    alBufferData(motorBuffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, &motorBuffer);
        return false;
    }
    //Unload sound file
    alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
    if ((error = alGetError()) != AL_NO_ERROR){
        alDeleteBuffers(1, &motorBuffer);
        return false;
    }
    #endif

    // create level
    world = new World(this);
    currentLevel = new Level(this);
    currentLevel->populate(world);

    airplane = world->getPlayer();

    airplane->getSceneNode()->attachObject(camera);
    airplane->setThrust(1000.0);
    
    display = new Display(this);
    display->setup();
    
    
    return true;
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
    // TODO
    std::cerr << "FAIL\n";
}
