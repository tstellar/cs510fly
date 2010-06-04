#include "Enemy.h"
#include "AirplaneEnemy.h"

AirplaneEnemy::AirplaneEnemy(Game * game, Ogre::SceneNode * sceneNode, const AirplaneState& state, Ogre::String name) :
        Enemy(game), airplane(new Airplane(game, sceneNode, state))
{
    Ogre::SceneManager * sceneManager = sceneNode->getCreator();
    Ogre::Entity * entity = sceneManager->createEntity(name, "sphere.mesh");
    entity->setMaterialName("Enemy_Material");
    sceneNode->scale(25.0f, 25.0f, 25.0f);
    sceneNode->attachObject(entity);

}

void AirplaneEnemy::update(Airplane * player, float dt) {
    if(posBelow(player->getPosition())){
        airplane->pitchDown();
    }else{
        airplane->pitchUp();
    }

    bool playerLeft = posLeft(player->getPosition());
    if(playerLeft){
        airplane->yawLeft();
    }else{
        airplane->yawRight();
    }

    if(!posInFront(player->getPosition())){
        airplane->decreaseThrust();
        if(playerLeft){
            airplane->rollRight();
        }
        else{
            airplane->rollLeft();
        }
    }
    airplane->update(dt);
}

AirplaneEnemy::~AirplaneEnemy() { }
