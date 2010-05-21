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

void Enemy::update(Airplane * player, float dt) {
    Ogre::Vector3& playerPosition = player->getPosition();
    Ogre::Vector3& enemyPosition = this->airplane->getPosition();
    /* TODO: not rolling, not pitching, and not yawing should be an option
     * if the enemy is within some constant amount of the player. */
    
    if(playerPosition.y < enemyPosition.y){
        airplane->pitchDown();
    }else{
        airplane->pitchUp();
    }

    if(playerPosition.x < enemyPosition.x){
        airplane->yawLeft();
    }else{
        airplane->yawRight();
    }

    /* These vectors only need two components, but I am using a Vector3 here
     * because the Vector3 class has better builtin fucntions and I am lazy.
     */
    Ogre::Vector3 enemyToPlayer = Ogre::Vector3(
                                        playerPosition.x - enemyPosition.x,
                                        playerPosition.z - enemyPosition.z, 0);
    Ogre::Vector3 enemyXZ = Ogre::Vector3(enemyPosition.x, enemyPosition.z, 0);
    Ogre::Radian angleBetween = enemyToPlayer.angleBetween(enemyXZ);

    /* Turn around to the right. */
    if(angleBetween > Ogre::Radian(Ogre::Math::HALF_PI)){
        airplane->decreaseThrust();
        airplane->rollRight();
        airplane->yawRight();
    }

    /* Turn around to the left. */
    if(angleBetween < Ogre::Radian(-Ogre::Math::HALF_PI)){
        airplane->decreaseThrust();
        airplane->rollLeft();
        airplane->yawLeft();
    }
    airplane->update(dt);
}

Enemy::~Enemy() { }
