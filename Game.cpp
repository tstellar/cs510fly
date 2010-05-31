// Game.cpp
// @author Eric D. Wills

#include "Game.h"

#include "Airplane.h"
#include "Display.h"
#include "InputListener.h"
#include "Level.h"
#include "World.h"

#include <AL/al.h>
#include <AL/alure.h>

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

    // create level
    world = new World(this);
    currentLevel = new Level(this);
    currentLevel->populate(world);

    airplane = world->getPlayer();

    airplane->getSceneNode()->attachObject(camera);
    airplane->setThrust(1000.0);
    
    display = new Display(this);
    display->setup();
    
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
    /* XXX What is the correct number of buffers, is it 1 per sound or 1 per
     * listener
     */
    ALuint buffers;
    alGenBuffers(1, &buffers);
    if ((error = alGetError()) != AL_NO_ERROR){
//        DisplayALError("alGenBuffers :", error);
        return false;
    }

    /* Load sound file */
    ALenum alFormatBuffer;
    char * alBuffer;
    long alBufferLen;
    ALsizei alFreqBuffer;
    ALboolean loop;
    buffers = alureCreateBufferFromFile("f15.wav");
/*    alutLoadWAVFile("f15.wav", &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer, &loop);
    if ((error = alGetError()) != AL_NO_ERROR){
//        DisplayALError("alutLoadWAVFile :", error);
        alDeleteBuffers(1, &buffers);
        return false;
    }
*/
    /* Copy sound to buffer */
/*    alBufferData(buffers[0], alFormatBuffer, alBuffer, alBufferLen, alFreqBuf);
    if ((error = alGetError()) != AL_NO_ERROR){
 //       DisplayALError("alBufferData :", error);
        alDeleteBuffers(1, &buffers);
        return false;
    }
*/
    /* Unload sound file */
/*    alutUnloadWav(alFormatBuffer, alBuffer, alBufferLen, alFreqBuf);
    if ((error = alGetError()) != AL_NO_ERROR){
//        DisplayALError("alutUnloadWav :", error);
        alDeleteBuffers(1, &buffers);
        return false;
    }
*/
    /* Generate sources */
    /* For multiple sources, this should be int * */
    unsigned int alSource;
    alGenSources(1, &alSource);
    if ((error = alGetError() ) != AL_NO_ERROR){
//        DisplayALError("alGenSources 1 : ", error);
        return false;
    }

    /* Attach buffer to a source */
    alSourcei(alSource, AL_BUFFER, buffers);
    if ((error = alGetError()) != AL_NO_ERROR){
//        DisplayALError("alSourcei : ", error);
        return false;
    }

    alSource3f(alSource,AL_POSITION, 800.0f, 0.0f, 800.0f);
    alSource3f(alSource,AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    
    alListener3f(AL_POSITION, 800.0f, 0.0f, 800.0f);

    /*XXX Set listener orientation */
    
    /* Set the sound to always loop */
    alSourcei(alSource,AL_LOOPING,AL_TRUE);
    
    /* play the sound */
    alSourcePlay(alSource);
    return true;
}

void Game::update(float dt) {
    world->update(dt);
    display->update(dt);
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
