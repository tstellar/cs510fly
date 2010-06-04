#include "Game.h"
#include "Airplane.h"

class AirplaneState;

class AirplaneEnemy : public Enemy{
    Airplane * airplane;
public:
    AirplaneEnemy(Game *, Ogre::SceneNode *, const AirplaneState&, Ogre::String);
    ~AirplaneEnemy();
    
    void update(Airplane *, float dt);
    const Ogre::Vector3& getPosition() { return airplane->getPosition(); }
    bool playerInFront(Airplane * player, const Ogre::Vector3& pos);
};
