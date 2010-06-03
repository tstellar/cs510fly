#include "World.h"

#include "Airplane.h"
#include "Target.h"
#include "Enemy.h"
#include "BasicEnemy.h"

const Ogre::String World::GROUND_NODE_NAME = "Ground";

World::World(Game * game) : Object(game, game->getSceneManager()->getRootSceneNode()), player(NULL), target(NULL) {

 
}

World::~World() {
    if (player != NULL)
        delete player;
    if (target != NULL)
        delete target;
    for (std::vector<Enemy *>::iterator it = enemies.begin(); it != enemies.end(); it++)
        delete *it;
}

Ogre::SceneNode * World::getRootNode() {
    return getSceneNode();
}

const Ogre::SceneNode * World::getRootNode() const {
    return getSceneNode();
}

Ogre::SceneNode * World::newNode(const Ogre::Vector3& position, const Ogre::Quaternion& orientation, const Ogre::String& name) {
    return getRootNode()->createChildSceneNode(name, position, orientation);
}

Airplane * World::addPlayer(const AirplaneState& state) {
    assert(player == NULL);
    
    Ogre::SceneNode * playerNode = newNode(state.position, state.orientation, "Player");
    player = new Airplane(game, playerNode, state);
    return player;
}

Target * World::addTarget(const Ogre::Vector3& position) {
    assert(target == NULL);

    Ogre::SceneNode * targetNode = newNode(position, Ogre::Quaternion::IDENTITY, "Target");
    target = new Target(targetNode);
    return target;
}

Enemy * World::addEnemy(const AirplaneState& state, const Ogre::String& name) {
    Ogre::SceneNode * enemyNode = newNode(state.position, state.orientation, name);
    Enemy * enemy = new BasicEnemy(game, enemyNode, state.position, name);
    enemies.push_back(enemy);
    return enemy;
}

void World::update(float dt) {
    std::vector<Enemy*>::iterator it;
    for (it = enemies.begin(); it != enemies.end(); ++it)
        (*it)->update(player, dt);
    player->update(dt);
}
