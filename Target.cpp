#include "Target.h"


Target::Target(Ogre::SceneManager *sceneManager, Ogre::Vector3 position){
	
	Ogre::Entity *entity = sceneManager->createEntity("Target","sphere.mesh");
	entity->setMaterialName("Target_Material");
	sceneNode = sceneManager->getRootSceneNode()->
				createChildSceneNode("Targeta", position);
	sceneNode->scale(100.0f, 100.0f, 100.0f);
	sceneNode->attachObject(entity);
	printf("Adding target at %lf %lf %lf\n",position.x, position.y, position.z);
}
