// Game.h
// @author Eric D. Wills

#ifndef __TERRAIN_DEMO_H__
#define __TERRAIN_DEMO_H__

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
    InputListener* inputListener;
    Ogre::Root* root;
    Ogre::SceneManager* sceneManager;
    Ogre::RenderWindow* renderWindow;
    Ogre::Camera* camera;
    Ogre::String mResourcePath;
    Ogre::String levelPath;
    Ogre::String audioPath;
    std::vector<Level *> levels;
    Level *currentLevel;
    World *world;
    Display * display;
    
    bool breaking;
    ALuint motorBuffer;
    ALuint enemyBuffer;


public:
    Game();

    ~Game();

    Ogre::SceneManager* getSceneManager() { return sceneManager; }
    Ogre::String getLevelPath() { return levelPath; }
    bool windowClosed() const { return renderWindow->isClosed(); }
    Airplane * getAirplane() { return airplane; }
    
    Level *getCurrentLevel(){ return currentLevel; };
    World * getWorld() { return world; }
    void init();
    void update(float dt);

    ALuint getMotorBuffer(){ return motorBuffer; }
    ALuint getEnemyBuffer(){ return enemyBuffer; }

    void setBreak();
    bool checkBreak();
    
    void startLevel(int index);
    void lose();
private:
    bool setup();
    bool loadWavFile(ALuint *buffer, std::string file);
};

#endif

