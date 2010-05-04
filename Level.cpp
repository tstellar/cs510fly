#include "Level.h"
#include "Target.h"

Level::Level(World * world) : world(world){
	
	Ogre::Vector3 targetPosition =
		Ogre::Vector3(700,world->getTerrainHeightAt(700,700),700);
	Ogre::SceneNode *targetNode = world->getSceneManager()->
		getRootSceneNode()->createChildSceneNode("Target", targetPosition);
	target = new Target(targetNode);
}
