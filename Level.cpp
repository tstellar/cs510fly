#include "Level.h"
#include "Target.h"

Level::Level(World * world) : world(world){
	Ogre::ConfigFile levelCFG;

	/*TODO: Specify the level to load. */
	levelCFG.load(world->getLevelPath() + "1.cfg");

	/* Load the Target's starting point. */
	Ogre::Vector2 targetXZ = Ogre::StringConverter::parseVector2(
					levelCFG.getSetting("Target"));
	float targetY = world->getTerrainHeightAt(targetXZ.x, targetXZ.y);
	Ogre::Vector3 targetPosition = Ogre::Vector3(targetXZ.x,targetY,targetXZ.y);

	/* Create the target. */
	Ogre::SceneNode *targetNode = world->getSceneManager()->
		getRootSceneNode()->createChildSceneNode("Target", targetPosition);
	target = new Target(targetNode);
}
