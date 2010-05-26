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
    OVERLAY_NAME = "Fly/HUD",
    PANEL_NAME = "Fly/HUDPanel",
    POSITION_TEXT_BOX_NAME = "Fly/Position",
    VELOCITY_TEXT_BOX_NAME = "Fly/Velocity";

Display::Display(Game * game) :
    game(game),
    overlay(Ogre::OverlayManager::getSingletonPtr()->getByName(OVERLAY_NAME)),
    panel(overlay->getChild(PANEL_NAME)),
    positionTextBox(dynamic_cast<Ogre::TextAreaOverlayElement *>(panel->getChild(POSITION_TEXT_BOX_NAME))),
    velocityTextBox(dynamic_cast<Ogre::TextAreaOverlayElement *>(panel->getChild(VELOCITY_TEXT_BOX_NAME))) { }

void Display::setup() {
    overlay->show();
}

void Display::update(float dt) {
    Airplane * const player = game->getWorld()->getPlayer();
    positionTextBox->setCaption(Ogre::StringConverter::toString(player->getPosition()));
    velocityTextBox->setCaption(Ogre::StringConverter::toString(player->getVelocity()));
}