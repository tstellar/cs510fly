#include "Game.h"
#include "Object.h"
#include "AirplaneState.h"

class Target;
class Enemy;
class Airplane;

class World : public Object {
    Airplane * player;
    Target * target;
    std::vector<Enemy *> enemies;

public:
    World(Game *);
    ~World();

    Airplane * addPlayer(const AirplaneState& airplaneState);
    Target * addTarget(const Ogre::Vector3& position);
    Enemy * addEnemy(const AirplaneState& airplaneState, const Ogre::String& name);

    void update(float);

    Ogre::SceneNode * getRootNode();
    const Ogre::SceneNode * getRootNode() const;
    
    Airplane * getPlayer() { return player; }
    const Airplane * getPlayer() const { return player; }
    
    static const Ogre::String GROUND_NODE_NAME;
    
private:
    Ogre::SceneNode * newNode(const Ogre::Vector3& position, const Ogre::Quaternion& orientation, const Ogre::String& name);
};
