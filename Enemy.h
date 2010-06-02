#include "Game.h"
#include "AirplaneState.h"

class Airplane;
class AI;

class Enemy{
    Game * const game;
    Airplane * airplane;
    AI * ai;

public:
    Enemy(Game *, Ogre::SceneNode *, const AirplaneState&, Ogre::String);
    Enemy(Game *, Ogre::SceneNode *, const Ogre::Vector3&, Ogre::String);
    ~Enemy();
    
    void update(Airplane *, float dt);

    bool playerBelow(Airplane * player, const Ogre::Vector3& pos);
    bool playerLeft(Airplane * player, const Ogre::Vector3& pos);
    bool playerInFront(Airplane * player, const Ogre::Vector3& pos);
};
