#include "PhysicalState.h"

static const Ogre::String
    POSITION_NAME = "Position",
    ORIENTATION_NAME = "Orientation",
    VELOCITY_NAME = "Velocity";

PhysicalState PhysicalState::readFromConfig(const Ogre::ConfigFile::SettingsMultiMap * settings) {
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    Ogre::Vector3 velocity;
    
    Ogre::ConfigFile::SettingsMultiMap::const_iterator iter;
    
    iter = settings->find(POSITION_NAME);
    if (iter == settings->end())
        position = Ogre::Vector3::ZERO;
    else
        position = Ogre::StringConverter::parseVector3(iter->second);
    
    iter = settings->find(ORIENTATION_NAME);
    if (iter == settings->end())
        orientation = Ogre::Quaternion::IDENTITY;
    else {
        const Ogre::Vector3 vec = Ogre::StringConverter::parseVector3(iter->second);
        const float pitch = vec.x, roll = vec.y, yaw = vec.z;
        orientation =
            Ogre::Quaternion(Ogre::Degree(roll), Ogre::Vector3::NEGATIVE_UNIT_Z) *
            Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X) *
            Ogre::Quaternion(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
    }
    
    iter = settings->find(VELOCITY_NAME);
    if (iter == settings->end())
        velocity = Ogre::Vector3::ZERO;
    else
        velocity = Ogre::StringConverter::parseVector3(iter->second);
    
    return PhysicalState(position, orientation, velocity);
}

void PhysicalState::syncToNode(Ogre::Node * node) const {
    node->setPosition(position);
    node->setOrientation(orientation);
}

void PhysicalState::syncFromNode(const Ogre::Node * node) {
    position = node->getPosition();
    orientation = node->getOrientation();
}

bool PhysicalState::clampAboveHeight(float height) {
    if (position.y < height) {
        position.y = height;
        return true;
    } else
        return false;
}
