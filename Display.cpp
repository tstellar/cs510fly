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
#include "Level.h"
#include "World.h"

static const Ogre::String
    TEXT_BOX_TEMPLATE_NAME = "Fly/TextBox",
    MAXED_TEXT_BOX_TEMPLATE_NAME = "Fly/MaxedTextBox",
    OVERLAY_NAME = "Fly/HUD",
    INFO_BAR_NAME = "Fly/InfoBar",
    LEVEL_TEXT_BOX_NAME = "Fly/LevelName",
    STATS_PANEL_NAME = "Fly/StatsPanel",
    POSITION_TEXT_BOX_NAME = "Fly/Position",
    VELOCITY_TEXT_BOX_NAME = "Fly/Velocity",
    PITCH_ROLL_YAW_TEXT_BOX_NAME = "Fly/PitchRollYaw",
    THRUST_TEXT_BOX_NAME = "Fly/Thrust",
    SPEED_TEXT_BOX_NAME = "Fly/Speed",
    AOA_TEXT_BOX_NAME = "Fly/AOA";

static Ogre::OverlayManager * overlayMgr() { return Ogre::OverlayManager::getSingletonPtr(); }

Display::Display(Game * game) :
    game(game),
    overlay(overlayMgr()->getByName(OVERLAY_NAME)),
    infoBar(overlay->getChild(INFO_BAR_NAME)),
    levelTextBox(getTextArea(LEVEL_TEXT_BOX_NAME, infoBar)),
    statsPanel(overlay->getChild(STATS_PANEL_NAME)),
    positionTextBox(getTextArea(POSITION_TEXT_BOX_NAME)),
    velocityTextBox(getTextArea(VELOCITY_TEXT_BOX_NAME)),
    pitchRollYawTextBox(getTextArea(PITCH_ROLL_YAW_TEXT_BOX_NAME)),
    thrustTextBox(getTextArea(THRUST_TEXT_BOX_NAME)),
    speedTextBox(getTextArea(SPEED_TEXT_BOX_NAME)),
    aoaTextBox(getTextArea(AOA_TEXT_BOX_NAME)) {

    Ogre::OverlayElement * elt = overlayMgr()->getOverlayElement(TEXT_BOX_TEMPLATE_NAME, true);
    normalValueColor = elt->getColour();
    
    elt = overlayMgr()->getOverlayElement(MAXED_TEXT_BOX_TEMPLATE_NAME, true);
    maxedValueColor = elt->getColour();
}

Ogre::TextAreaOverlayElement * Display::getTextArea(Ogre::String name, Ogre::OverlayContainer * parent) {
    return dynamic_cast<Ogre::TextAreaOverlayElement *>((parent == NULL ? statsPanel : parent)->getChild(name));
}

void Display::setup() {
    overlay->show();
}

// Shortcut to cut down on absurdly lengthy output code
template<typename T> static inline Ogre::String toString(T x) { return Ogre::StringConverter::toString(x); }

void Display::update(float dt) {
    const Airplane * const player = game->getWorld()->getPlayer();
    const AirplaneState& state = player->getState();
    levelTextBox->setCaption(game->getCurrentLevel()->getName());
    positionTextBox->setCaption(toString(state.position));
    velocityTextBox->setCaption(toString(state.velocity));
    speedTextBox->setCaption(toString(state.velocity.length()));
    aoaTextBox->setCaption(toString(state.angleOfAttack().valueDegrees()));
    
    const Ogre::Degree pitch = player->getPitch(), roll = player->getRoll(), yaw = player->getYaw();
    pitchRollYawTextBox->setCaption(toString(pitch) + " " + toString(roll) + " " + toString(yaw));
    
    thrustTextBox->setCaption(toString(state.thrust));
    thrustTextBox->setColour(player->atMaximumThrust() ? maxedValueColor : normalValueColor);
}
