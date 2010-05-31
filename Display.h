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
    Ogre::TextAreaOverlayElement * const pitchRollYawTextBox;
    Ogre::TextAreaOverlayElement * const thrustTextBox;
    
    Ogre::ColourValue normalValueColor, maxedValueColor;

public:
    Display(Game *);
    
    void setup();
    void update(float dt);
    
private:
    Ogre::TextAreaOverlayElement * getTextArea(Ogre::String name);
};

#endif
