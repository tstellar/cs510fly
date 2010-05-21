#include "Ogre.h"

class Game;
class Airplane;

class Enemy{
    Game * const game;
    Airplane * airplane;

public:
    Enemy(Game *, Ogre::SceneNode *, Ogre::String);
    ~Enemy();
    void update(float dt);
};
