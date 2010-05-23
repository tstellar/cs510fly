#include "Enemy.h"
#include "Level.h"
#include "Target.h"
#include "World.h"

Level::Level(Game * game) : game(game){
    Ogre::ConfigFile levelCFG;

    /*TODO: Specify the level to load. */
    levelCFG.load(game->getLevelPath() + "1.cfg");

    /* Load the Target's starting point. */
    Ogre::Vector2 targetXZ = Ogre::StringConverter::parseVector2(
                    levelCFG.getSetting("Target"));
    float targetY = game->getTerrainHeightAt(targetXZ.x, targetXZ.y);
    targetStart = Ogre::Vector3(targetXZ.x,targetY,targetXZ.y);

    /* Load the player's starting point. */
    Ogre::Vector2 playerXZ = Ogre::StringConverter::parseVector2(
                    levelCFG.getSetting("Player"));
    float playerY = game->getTerrainHeightAt(playerXZ.x, playerXZ.y);
    playerStart = Ogre::Vector3(playerXZ.x, playerY, playerXZ.y);

    /* Create the enemies. */
    Ogre::ConfigFile::SectionIterator sit = levelCFG.getSectionIterator();
    while(sit.hasMoreElements()){
        Ogre::String sectionName = sit.peekNextKey();
        Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(
            levelCFG.getSetting("Position", sectionName));
        pos.y += game->getTerrainHeightAt(pos.x, pos.z);
        enemyStarts.push_back(pos);
        enemyNames.push_back(sectionName);
        sit.moveNext();
    }

}


void Level::populate(World * world) const {
    // TODO Set orientations properly
    world->addPlayer(playerStart, Ogre::Quaternion::IDENTITY);
    world->addTarget(targetStart, Ogre::Quaternion::IDENTITY);

    std::vector<Ogre::Vector3>::const_iterator posIter = enemyStarts.begin();
    std::vector<Ogre::String>::const_iterator nameIter = enemyNames.begin();

    for (; posIter != enemyStarts.end(); posIter++, nameIter++)
        world->addEnemy(*posIter, Ogre::Quaternion::IDENTITY, *nameIter);
}

