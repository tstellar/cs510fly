#include "Ogre.h"

class World;
class Airplane;

class Enemy{
    World * const world;
    Airplane * airplane;

public:
    Enemy(World *, Ogre::SceneNode *, Ogre::String);
    ~Enemy();
    void update(float dt);
};
