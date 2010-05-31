/*
 *  Display.cpp
 *  Fly
 *
 *  Created by Luke Maurer on 5/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Display.h"

#include "Airplane.h"
#include "World.h"

static const Ogre::String
    TEXT_BOX_TEMPLATE_NAME = "Fly/TextBox",
    MAXED_TEXT_BOX_TEMPLATE_NAME = "Fly/MaxedTextBox",
    OVERLAY_NAME = "Fly/HUD",
    PANEL_NAME = "Fly/HUDPanel",
    POSITION_TEXT_BOX_NAME = "Fly/Position",
    VELOCITY_TEXT_BOX_NAME = "Fly/Velocity",
    PITCH_ROLL_YAW_TEXT_BOX_NAME = "Fly/PitchRollYaw",
    THRUST_TEXT_BOX_NAME = "Fly/Thrust";

static Ogre::OverlayManager * overlayMgr() { return Ogre::OverlayManager::getSingletonPtr(); }

Display::Display(Game * game) :
    game(game),
    overlay(overlayMgr()->getByName(OVERLAY_NAME)),
    panel(overlay->getChild(PANEL_NAME)),
    positionTextBox(getTextArea(POSITION_TEXT_BOX_NAME)),
    velocityTextBox(getTextArea(VELOCITY_TEXT_BOX_NAME)),
    pitchRollYawTextBox(getTextArea(PITCH_ROLL_YAW_TEXT_BOX_NAME)),
    thrustTextBox(getTextArea(THRUST_TEXT_BOX_NAME)) {

    Ogre::OverlayElement * elt = overlayMgr()->getOverlayElement(TEXT_BOX_TEMPLATE_NAME, true);
    normalValueColor = elt->getColour();
    
    elt = overlayMgr()->getOverlayElement(MAXED_TEXT_BOX_TEMPLATE_NAME, true);
    maxedValueColor = elt->getColour();
}

Ogre::TextAreaOverlayElement * Display::getTextArea(Ogre::String name) {
    return dynamic_cast<Ogre::TextAreaOverlayElement *>(panel->getChild(name));
}

void Display::setup() {
    overlay->show();
}

// Shortcut to cut down on absurdly lengthy output code
template<typename T> static inline Ogre::String toString(T x) { return Ogre::StringConverter::toString(x); }

void Display::update(float dt) {
    Airplane * const player = game->getWorld()->getPlayer();
    positionTextBox->setCaption(toString(player->getPosition()));
    velocityTextBox->setCaption(toString(player->getVelocity()));
    
    const Ogre::Degree pitch = player->getPitch(), roll = player->getRoll(), yaw = player->getYaw();
    pitchRollYawTextBox->setCaption(toString(pitch) + " " + toString(roll) + " " + toString(yaw));
    
    thrustTextBox->setCaption(toString(player->getThrust()));
    thrustTextBox->setColour(player->atMaximumThrust() ? maxedValueColor : normalValueColor);
}
