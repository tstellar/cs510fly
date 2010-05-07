#include "Enemy.h"
#include "Airplane.h"

Enemy::Enemy(World * world, Ogre::SceneNode * sceneNode, Ogre::String name) :
		world(world), airplane(new Airplane(world, sceneNode)) {
	Ogre::SceneManager * sceneManager = sceneNode->getCreator();
	Ogre::Entity * entity = sceneManager->createEntity(name, "sphere.mesh");
	entity->setMaterialName("Enemy_Material");
	sceneNode->scale(25.0f, 25.0f, 25.0f);
	sceneNode->attachObject(entity);
}

void Enemy::update(float dt) {

}

Enemy::~Enemy() { }
