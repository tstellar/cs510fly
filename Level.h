#include "World.h"

class Target;

class Level{
	World * world;
	Target * target;
	Ogre::Vector3 playerStart;
public:
	Level(World *);
	Ogre::Vector3& getPlayerStart(){ return playerStart; }
};
