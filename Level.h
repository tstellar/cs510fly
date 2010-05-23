#include "Game.h"
#include <vector>

class Target;
class Enemy;

class Level{
    Game * const game;
    Ogre::Vector3 playerStart, targetStart;
    std::vector<Ogre::Vector3> enemyStarts;
    std::vector<Ogre::String> enemyNames;
public:
    Level(Game *);
    ~Level();

    void populate(World * world) const;
};
