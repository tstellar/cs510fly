#ifndef DISPLAY_H
#define DISPLAY_H

#include "OgreTextAreaOverlayElement.h"

#include "Game.h"

class Display {
    Game * const game;
    Ogre::Overlay * const overlay;
    
    Ogre::OverlayContainer * const infoBar;
    Ogre::TextAreaOverlayElement * const levelTextBox;
    Ogre::TextAreaOverlayElement * const distanceTextBox;

    Ogre::OverlayContainer * const statsPanel;
    Ogre::TextAreaOverlayElement * const positionTextBox;
    Ogre::TextAreaOverlayElement * const velocityTextBox;
    Ogre::TextAreaOverlayElement * const pitchRollYawTextBox;
    Ogre::TextAreaOverlayElement * const thrustTextBox;
    Ogre::TextAreaOverlayElement * const speedTextBox;
    Ogre::TextAreaOverlayElement * const aoaTextBox;
    
    Ogre::ColourValue normalValueColor, maxedValueColor;

public:
    Display(Game *);
    
    void setup();
    void update(float dt);
    
private:
    Ogre::TextAreaOverlayElement * getTextArea(
            Ogre::String name, Ogre::OverlayContainer * parent = NULL);
};

#endif
