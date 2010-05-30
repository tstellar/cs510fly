#include "Game.h"
#include "PhysicalState.h"

class Airplane;

class Enemy{
    Game * const game;
    Airplane * airplane;

public:
    Enemy(Game *, Ogre::SceneNode *, const PhysicalState&, Ogre::String);
    ~Enemy();

    void update(Airplane *, float dt);
};
