#include "Game.h"

class Target;

class Level{
    Game * const game;
    Target * target;
    Ogre::Vector3 playerStart;
public:
    Level(Game *);
    ~Level();
    Ogre::Vector3& getPlayerStart(){ return playerStart; }
};
