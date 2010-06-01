#include "Game.h"
#include "AirplaneState.h"
#include <list>

class Target;
class Enemy;

class Level{
    Game * const game;
    Ogre::Vector3 targetStart;
    AirplaneState playerState;
    std::list<std::pair<const Ogre::String, const AirplaneState> > enemyStates;
public:
    Level(Game *);

    void populate(World * world) const;
    
    static const Ogre::String PLAYER_SECTION_NAME;
private:
    void createGroundMesh() const;
};
