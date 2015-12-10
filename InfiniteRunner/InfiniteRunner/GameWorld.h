#pragma once
#ifndef GAME_WORLD
#define GAME_WORLD

#include "BaseObject.h"
#include "Skybox.h"

#include <vector>

class GameObjectManager;
class Camera;
class PCInputSystem;
class LightSource;
class TheGame;

class GameWorld : public BaseObject
{
protected:
	GameObjectManager *objectManager;
	Camera *camera;
	PCInputSystem *inputSystem;
	TheGame* theGame;
	Skybox* skybox;

public:
	std::vector<LightSource *> localLights;

public:
	GameWorld( GameObjectManager *objectManager, Camera *camera, Logger* logger );

	virtual ~GameWorld();

	GameObjectManager *getObjectManager() { return this->objectManager; }

	Camera *getCamera() { return this->camera; }

	void setTheGame( TheGame* theGame ) { this->theGame = theGame; }

	void setInputSystem( PCInputSystem* inputSystem ) { this->inputSystem = inputSystem; }

	void setSkybox( Skybox* skybox ) { this->skybox = skybox; }

	void renderSkybox() { this->skybox->render(); }

	void updateSkybox();

	virtual void update( float elapsedSeconds );

	virtual void processInputs();

protected:

};

#endif

