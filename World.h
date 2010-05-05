// World.h
// @author Eric D. Wills

#ifndef __TERRAIN_DEMO_H__
#define __TERRAIN_DEMO_H__

#include "Ogre.h"

#ifndef LINUX
#include <CoreFoundation/CoreFoundation.h>
#endif


class InputListener;
class Level;

class World {
 private:
	InputListener* inputListener;
	Ogre::Root* root;
	Ogre::SceneManager* sceneManager;
	Ogre::RenderWindow* renderWindow;
	Ogre::Camera* camera;
	Ogre::RaySceneQuery* raySceneQuery;
	Ogre::Ray terrainRay;
	Ogre::String mResourcePath;
	Level *currentLevel;

 public:
	World();

	~World();

	Ogre::Root* getRoot() { return root; }
	Ogre::SceneManager* getSceneManager() { return sceneManager; }
	Ogre::RenderWindow* getRenderWindow() { return renderWindow; }
	Ogre::Camera* getCamera() { return camera; }
	Ogre::String getResourcePath() { return mResourcePath; }
	float getTerrainHeightAt(float x, float y);

	void adjustCameraHeightToTerrain();

	void init();

 private:
	bool setup();
};

#endif
