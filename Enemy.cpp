#include "Enemy.h"
#include "Airplane.h"
#include "AirplaneAI.h"
#include "EnemyAI.h"

Enemy::Enemy(Game * game, Ogre::SceneNode * sceneNode, const AirplaneState& state, Ogre::String name) :
        game(game),
        ai(new AirplaneAI(new Airplane(game, sceneNode, state)))
{
    Ogre::SceneManager * sceneManager = sceneNode->getCreator();
    Ogre::Entity * entity = sceneManager->createEntity(name, "sphere.mesh");
    entity->setMaterialName("Enemy_Material");
    sceneNode->scale(25.0f, 25.0f, 25.0f);
    sceneNode->attachObject(entity);

}

Enemy::Enemy(Game * game, Ogre::SceneNode * sceneNode,
    const Ogre::Vector3& position, Ogre::String name) :
        game(game), ai(new EnemyAI(sceneNode))
{
    Ogre::SceneManager * sceneManager = sceneNode->getCreator();
    Ogre::Entity * entity = sceneManager->createEntity(name, "sphere.mesh");
    entity->setMaterialName("Enemy_Material");
    sceneNode->scale(25.0f, 25.0f, 25.0f);
    sceneNode->attachObject(entity);
    sceneNode->setPosition(position);
}
/*These functions should either be static or moved elsewhere. */
bool Enemy::playerBelow(Airplane * player, const Ogre::Vector3& pos) {
	return player->getPosition().y < pos.y;
}

bool Enemy::playerLeft(Airplane * player, const Ogre::Vector3& pos) {
	return player->getPosition().x < pos.x;
}

bool Enemy::playerInFront(Airplane * player, const Ogre::Vector3& pos) {
    const Ogre::Vector3& playerPosition = player->getPosition();
    /* These vectors only need two components, but I am using a Vector3 here
     * because the Vector3 class has better builtin fucntions and I am lazy.
     */
    Ogre::Vector3 enemyToPlayer = Ogre::Vector3(
                                        playerPosition.x - pos.x,
                                        playerPosition.z - pos.z, 0);
    Ogre::Vector3 posXZ = Ogre::Vector3(pos.x, pos.z, 0);
    const Ogre::Radian& angleBetween = enemyToPlayer.angleBetween(posXZ);
    return Ogre::Math::Abs(angleBetween) < Ogre::Radian(Ogre::Math::HALF_PI);; 
}

void Enemy::update(Airplane * player, float dt) {
    ai->update(this, player, dt);
}

Enemy::~Enemy() { }
