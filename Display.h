#ifndef DISPLAY_H
#define DISPLAY_H

#include "OgreTextAreaOverlayElement.h"

#include "Game.h"

class Display {
    Game * const game;
    Ogre::Overlay * const overlay;
    Ogre::OverlayContainer * const panel;
    Ogre::TextAreaOverlayElement * const positionTextBox;
    Ogre::TextAreaOverlayElement * const velocityTextBox;

public:
    Display(Game *);
    
    void setup();
    void update(float dt);
};

#endif