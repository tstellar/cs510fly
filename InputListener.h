// InputListener.h
// @author Eric D. Wills

#ifndef __TERRAIN_DEMO_FRAME_LISTENER_H__
#define __TERRAIN_DEMO_FRAME_LISTENER_H__

#include "Ogre.h"
#include "OIS/OIS.h"

class Game;

class InputListener : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener {
private:
    Game* const game;
    Ogre::RenderWindow * const renderWindow;
    OIS::InputManager* inputManager;
    OIS::Mouse* mouse;
    OIS::Keyboard* keyboard;
    bool forwardKeyDown, backKeyDown, upKeyDown, downKeyDown,
            leftKeyDown, rightKeyDown, yawLeftKeyDown, yawRightKeyDown;
    bool shutdownKeyPressed;

public:
    InputListener(Game* game, Ogre::RenderWindow * renderWindow);

    ~InputListener();

    bool frameStarted(const Ogre::FrameEvent& event);
    bool frameEnded(const Ogre::FrameEvent& event);

    bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
    bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
    bool mouseMoved(const OIS::MouseEvent& event);

    bool keyPressed(const OIS::KeyEvent& event);
    bool keyReleased(const OIS::KeyEvent& event);
};

#endif

