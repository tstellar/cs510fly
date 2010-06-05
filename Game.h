// Game.h
// @author Eric D. Wills

#ifndef __TERRAIN_DEMO_H__
#define __TERRAIN_DEMO_H__

#include <tr1/memory>

#include "Ogre.h"

#ifndef LINUX
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef LINUX
    #include <AL/al.h>
#else
    #include <OpenAL/al.h>
#endif

class Airplane;
class Display;
class InputListener;
class Level;
class World;

class Game {
private:
    Airplane * airplane;
    std::auto_ptr<InputListener> inputListener;
    std::auto_ptr<Ogre::Root> root;
    Ogre::SceneManager * sceneManager;
    Ogre::RenderWindow * renderWindow;
    Ogre::SceneNode * cameraNode;
    Ogre::Camera * camera;
    Ogre::String mResourcePath;
    Ogre::String levelPath;
    Ogre::String audioPath;
    std::vector<std::tr1::shared_ptr<const Level> > levels;
    int currentLevelIndex;
    const Level *currentLevel;
    std::auto_ptr<World> world;
    std::auto_ptr<Display> display;
    
    bool breaking;
    ALuint motorBuffer;
    ALuint enemyBuffer;


public:
    Game();

    Ogre::SceneManager* getSceneManager() { return sceneManager; }
    Ogre::String getLevelPath() { return levelPath; }
    bool windowClosed() const { return renderWindow->isClosed(); }
    Airplane * getAirplane() { return airplane; }
    
    const Level *getCurrentLevel(){ return currentLevel; };
    World * getWorld() { return world.get(); }
    void init();
    void update(float dt);

    ALuint getMotorBuffer(){ return motorBuffer; }
    ALuint getEnemyBuffer(){ return enemyBuffer; }

    void setBreak();
    bool checkBreak();
    
    void startLevel(int index);
    void lose();
    void win();
private:
    bool setup();
    bool loadWavFile(ALuint *buffer, std::string file);
};

#endif

