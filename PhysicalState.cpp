#include "PhysicalState.h"

void PhysicalState::syncToNode(Ogre::Node * node) const {
    node->setPosition(position);
    node->setOrientation(orientation);
}

void PhysicalState::syncFromNode(const Ogre::Node * node) {
    position = node->getPosition();
    orientation = node->getOrientation();
}
