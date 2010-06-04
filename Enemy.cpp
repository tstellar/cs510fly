#include "Enemy.h"

static const float DEATH_RANGE_SQUARED = 100.0f;

Enemy::Enemy(Game * const game) : game(game) { }

bool Enemy::inRange(const Ogre::Vector3& pos) {
    return pos.squaredDistance(getPosition()) <= DEATH_RANGE_SQUARED;
}

bool Enemy::posBelow(const Ogre::Vector3& pos) {
	return pos.y < getPosition().y;
}

bool Enemy::posLeft(const Ogre::Vector3& pos) {
	return pos.x < getPosition().x;
}

bool Enemy::posInFront(const Ogre::Vector3& pos) {
    const Ogre::Vector3& enemyPos = getPosition();
    /* These vectors only need two components, but I am using a Vector3 here
     * because the Vector3 class has better builtin fucntions and I am lazy.
     */
    Ogre::Vector3 enemyToPlayer = Ogre::Vector3(
                                        pos.x - enemyPos.x,
                                        pos.z - enemyPos.z, 0);
    Ogre::Vector3 enemyPosXZ = Ogre::Vector3(enemyPos.x, enemyPos.z, 0);
    const Ogre::Radian& angleBetween = enemyToPlayer.angleBetween(enemyPosXZ);
    return Ogre::Math::Abs(angleBetween) < Ogre::Radian(Ogre::Math::HALF_PI);; 
}
