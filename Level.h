#include "Game.h"
#include <vector>

class Target;
class Enemy;

class Level{
    Game * const game;
    Target * target;
    Ogre::Vector3 playerStart;
    std::vector<Enemy*> enemies;
public:
    Level(Game *);
    ~Level();

    Ogre::Vector3& getPlayerStart(){ return playerStart; }
    
    void update(float);
};
