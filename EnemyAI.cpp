#include "EnemyAI.h"
#include "Enemy.h"
#include "Airplane.h"

#include "Ogre.h"

EnemyAI::EnemyAI(Ogre::SceneNode *sceneNode) : sceneNode(sceneNode),
   velocity(0.0f), difficulty(1)
{
    
}

void EnemyAI::update(Enemy * enemy, Airplane * player, float dt){
    
    velocity += ((float)difficulty) * dt ;
    const Ogre::Vector3& currentPosition = sceneNode->getPosition();
    Ogre::Vector3 direction = (player->getPosition() - currentPosition);
    direction = ((velocity * dt) * direction) / 
        sqrt(direction.dotProduct(direction));
        sceneNode->translate(direction);
}
