#include "Enemy.h"
#include "BasicEnemy.h"
#include "Airplane.h"

BasicEnemy::BasicEnemy(Game * game, Ogre::SceneNode * sceneNode,
    const Ogre::Vector3& position, Ogre::String name) : Enemy(game),
        sceneNode(sceneNode), velocity(0.0f), difficulty(1)
{
    Ogre::SceneManager * sceneManager = this->sceneNode->getCreator();
    Ogre::Entity * entity = sceneManager->createEntity(name, "sphere.mesh");
    entity->setMaterialName("Enemy_Material");
    this->sceneNode->scale(25.0f, 25.0f, 25.0f);
    this->sceneNode->attachObject(entity);
    this->sceneNode->setPosition(position);
    
    Ogre::ParticleSystem * engineParticles = sceneNode->getCreator()->createParticleSystem(sceneNode->getName() + "Engine", "Engine");
    sceneNode->attachObject(engineParticles);

    ALenum error;
    /* Setup Sound */
    alGenSources(1, &alSource);
    if((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "Error creating OpenAL source. %d\n",error);
    }
    alSourcei(alSource, AL_BUFFER, game->getEnemyBuffer());
    if((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "Error attching enemy buffer to source. %d\n",error);
    }
    alSourcei(alSource, AL_LOOPING, AL_TRUE);
    alSourcePlay(alSource);
}

void BasicEnemy::update(Airplane * player, float dt) {
    
    velocity += ((float)difficulty) * dt ;
    const Ogre::Vector3& originalPosition = sceneNode->getPosition();
    Ogre::Vector3 direction = (player->getPosition() - originalPosition);
    direction = ((velocity * dt) * direction) / 
        sqrt(direction.dotProduct(direction));
    sceneNode->translate(direction);
   
    const Ogre::Vector3& newPosition = sceneNode->getPosition();
    const Ogre::Vector3& velocityVec = (dt == 0.0f ? Ogre::Vector3::ZERO : (newPosition - originalPosition) / dt);
    alSource3f(alSource, AL_POSITION, newPosition.x, newPosition.y, newPosition.z);
    alSource3f(alSource, AL_VELOCITY, velocityVec.x, velocityVec.y, velocityVec.z);
}

BasicEnemy::~BasicEnemy() { }
