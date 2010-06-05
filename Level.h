#include "Game.h"
#include "AirplaneState.h"
#include <list>

class Target;
class Enemy;

class Level{
    Game * const game;
    Ogre::String name;
    Ogre::String skybox;
    Ogre::Vector3 targetStart;
    AirplaneState playerState;
    std::list<std::pair<const Ogre::String, const AirplaneState> > enemyStates;
public:
    Level(Game *, const Ogre::String& filename);
    
    const Ogre::String& getName() const { return name; }
    const Ogre::String& getSkyBox() const { return skybox; } 
    void populate(World * world) const;
    
    static const Ogre::String PLAYER_SECTION_NAME;
private:
    void createGroundMesh() const;
};
