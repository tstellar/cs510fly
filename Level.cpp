#include "Enemy.h"
#include "Level.h"
#include "Target.h"

Level::Level(Game * game) : game(game){
    Ogre::ConfigFile levelCFG;

    /*TODO: Specify the level to load. */
    levelCFG.load(game->getLevelPath() + "1.cfg");

    /* Load the Target's starting point. */
    Ogre::Vector2 targetXZ = Ogre::StringConverter::parseVector2(
                    levelCFG.getSetting("Target"));
    float targetY = game->getTerrainHeightAt(targetXZ.x, targetXZ.y);
    Ogre::Vector3 targetPosition = Ogre::Vector3(targetXZ.x,targetY,targetXZ.y);

    /* Load the player's starting point. */
    Ogre::Vector2 playerXZ = Ogre::StringConverter::parseVector2(
                    levelCFG.getSetting("Player"));
    float playerY = game->getTerrainHeightAt(playerXZ.x, playerXZ.y);
    playerStart = Ogre::Vector3(playerXZ.x, playerY, playerXZ.y);

    /* Create the target. */
    Ogre::SceneNode *targetNode = game->getSceneManager()->
        getRootSceneNode()->createChildSceneNode("Target", targetPosition);
    target = new Target(targetNode);
    
    /* Create the enemies. */
    Ogre::ConfigFile::SectionIterator sit = levelCFG.getSectionIterator();
    while(sit.hasMoreElements()){
        Ogre::String sectionName = sit.peekNextKey();
        Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(
            levelCFG.getSetting("Position", sectionName));
        pos.y += game->getTerrainHeightAt(pos.x, pos.z);
        Ogre::SceneNode *enemyNode = game->getSceneManager()->
            getRootSceneNode()->createChildSceneNode(sectionName, pos);
        enemies.push_back(new Enemy(game, enemyNode, sectionName));
        sit.moveNext();
    }

}

Level::~Level(){
    delete target;
}

void Level::update(float dt){
    std::vector<Enemy*>::iterator it;
    for( it = enemies.begin(); it != enemies.end(); ++it){
        (*it)->update(game->getAirplane(), dt);
    }
}

