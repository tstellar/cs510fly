#include "Game.h"
#include "AirplaneState.h"

class Airplane;

class Enemy{
    Game * const game;
    Airplane * airplane;

public:
    Enemy(Game *, Ogre::SceneNode *, const AirplaneState&, Ogre::String);
    ~Enemy();

    void update(Airplane *, float dt);
};
