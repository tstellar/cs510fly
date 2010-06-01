#include "Game.h"
#include "AirplaneState.h"
#include <list>

class Target;
class Enemy;

class Level{
    Game * const game;
    Ogre::Vector3 targetStart;
    PhysicalState playerStart;
    std::list<PhysicalState> enemyStarts;
    std::list<Ogre::String> enemyNames;
public:
    Level(Game *);

    void populate(World * world) const;
    
    static const Ogre::String GROUND_NODE_NAME;
private:
    void createGroundMesh() const;
};
