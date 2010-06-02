#include "AirplaneAI.h"
#include "Airplane.h"
#include "Enemy.h"

AirplaneAI::AirplaneAI(Airplane * airplane): airplane(airplane)
{
    
}

void AirplaneAI::update(Enemy * enemy, Airplane * player, float dt){
    
    if(enemy->playerBelow(player, airplane->getPosition())){
        airplane->pitchDown();
    }else{
        airplane->pitchUp();
    }

    bool playerLeft = enemy->playerLeft(player, airplane->getPosition());
    if(playerLeft){
        airplane->yawLeft();
    }else{
        airplane->yawRight();
    }

    if(!enemy->playerInFront(player, airplane->getPosition())){
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
