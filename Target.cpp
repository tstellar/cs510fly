#include "Target.h"


Target::Target(Ogre::SceneManager *sceneManager, Ogre::Vector3 position){
	
	Ogre::Entity *entity = sceneManager->createEntity("Target",
		Ogre::SceneManager::PT_PLANE);
	entity->setMaterialName("Target_Material");
	sceneNode = sceneManager->getRootSceneNode()->
				createChildSceneNode("Target", position);
	sceneNode->scale(100.0f, 100.0f, 100.0f);
	sceneNode->attachObject(entity);
}
