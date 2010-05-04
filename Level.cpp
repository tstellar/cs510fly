#include "Level.h"
#include "Target.h"

Level::Level(World * world) : world(world){
	
	target = new Target(world->getSceneManager(),
						Ogre::Vector3(700,world->getTerrainHeightAt(700,700),700));
}
