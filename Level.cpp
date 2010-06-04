#include "ConfigReader.h"
#include "Enemy.h"
#include "Level.h"
#include "Target.h"
#include "World.h"

const Ogre::String Level::PLAYER_SECTION_NAME = "$PLAYER";

Level::Level(Game * game, const Ogre::String& filename) : game(game){
    ConfigReader config(filename);
    
    name = config.get("Name", filename);
    
    /* Load the Target's starting point. */
    Ogre::Vector2 targetXZ = config.parse("Target", &Ogre::StringConverter::parseVector2, Ogre::Vector2::ZERO);
    targetStart = Ogre::Vector3(targetXZ.x,0,targetXZ.y);

    /* Create the enemies and the player start. */
    config.advanceSection();
    for (; config.hasNextSection(); config.advanceSection()){
        const Ogre::String& sectionName = config.getSectionName();
        
        const AirplaneState& state = AirplaneState::readFromConfig(&config);
        if (sectionName == PLAYER_SECTION_NAME)
            playerState = state;
        else {
            enemyStates.push_back(std::make_pair(sectionName, state));
        }
    }

}

void Level::createGroundMesh() const {
    Ogre::MeshManager * meshMgr = Ogre::MeshManager::getSingletonPtr();
    meshMgr->createPlane("Ground", "Meshes", Ogre::Plane(Ogre::Vector3::UNIT_Y, 0.0f), 100000, 100000,
                         100, 100, true, 1, 1000, 1000, Ogre::Vector3::UNIT_X);
}

void Level::populate(World * world) const {
    world->addPlayer(playerState);
    world->addTarget(targetStart);

    std::list<std::pair<const Ogre::String, const AirplaneState> >::const_iterator iter = enemyStates.begin();

    for (; iter != enemyStates.end(); iter++)
        world->addEnemy(iter->second, iter->first);
    
    Ogre::SceneNode * const root = world->getRootNode();
    Ogre::SceneManager * const sceneMgr = root->getCreator();
    
    Ogre::SceneNode * const ground = root->createChildSceneNode(World::GROUND_NODE_NAME);
    createGroundMesh();
    Ogre::Entity * const groundEntity = sceneMgr->createEntity(World::GROUND_NODE_NAME, "Ground");
    groundEntity->setMaterialName("Ground_Material");
    ground->attachObject(groundEntity);
}

