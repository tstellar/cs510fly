#include "World.h"
#include <vector>

class Target;
class Enemy;

class Level{
    World * world;
    Target * target;
    Ogre::Vector3 playerStart;
    std::vector<Enemy*> enemies;
public:
    Level(World *);
    ~Level();

    Ogre::Vector3& getPlayerStart(){ return playerStart; }
    
    void update(float);
};
