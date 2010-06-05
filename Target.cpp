#include "Target.h"


Target::Target(Ogre::SceneNode * sceneNode) : sceneNode(sceneNode){
    
    Ogre::SceneManager * sceneManager = sceneNode->getCreator();
    Ogre::Entity *entity = sceneManager->createEntity("Target","sphere.mesh");
    entity->setMaterialName("Target_Material");
    sceneNode->scale(100.0f, 100.0f, 100.0f);
    sceneNode->attachObject(entity);
    Ogre::ParticleSystem * particles = sceneNode->getCreator()->createParticleSystem(sceneNode->getName() + "Target", "Target");
    sceneNode->attachObject(particles);
}

Target::~Target() { }
