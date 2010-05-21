// World.cpp
// @author Eric D. Wills

#include "World.h"

#include "Airplane.h"
#include "InputListener.h"
#include "Level.h"

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

World::World() : inputListener(NULL), raySceneQuery(NULL), airplane(NULL),
        currentLevel(NULL) {
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

World::~World() {
    if (inputListener != NULL){
        delete inputListener;
    }
    if (raySceneQuery != NULL){
        delete raySceneQuery;
    }
    if (airplane != NULL){
        delete airplane;
    }
    if (currentLevel != NULL){
        delete currentLevel;
    }
    delete root;
}


float World::getTerrainHeightAt(float x, float z) {
    terrainRay.setOrigin(Ogre::Vector3(x, 1000.0f, z));
    raySceneQuery->setRay(terrainRay);
    Ogre::RaySceneQueryResult&  queryResult = raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator qi = queryResult.begin();
    return (qi != queryResult.end() && qi->worldFragment) ? 
            qi->worldFragment->singleIntersection.y : 0.0f;
}

void World::init() {
    if (setup()){
        root->startRendering();
    }
}

bool World::setup() {
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
    sceneManager = root->createSceneManager("TerrainSceneManager");

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
    sceneManager->setWorldGeometry("terrain.cfg");

    // set up the ray query for terrain following
    terrainRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

    raySceneQuery = sceneManager->createRayQuery(terrainRay);

    inputListener = new InputListener(this, renderWindow);
    root->addFrameListener(inputListener);

    // create level
    currentLevel = new Level(this);

    // create airplane node
    Ogre::SceneNode * airplaneNode = sceneManager->createSceneNode(Airplane::SCENE_NODE_NAME);
    airplaneNode->setPosition(currentLevel->getPlayerStart());
    airplaneNode->attachObject(camera);


    airplane = new Airplane(this, airplaneNode);
    airplane->setThrust(1000.0);

    return true;
}

void World::update(float dt){

    airplane->update(dt);
    currentLevel->update(dt);
}


