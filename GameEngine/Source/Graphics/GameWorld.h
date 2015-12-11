#pragma once
#ifndef GAME_WORLD
#define GAME_WORLD

#include <vector>
using std::vector;

class GameObjectManager;
class OGLFirstPersonCamera;
class PCInputSystem;
class TheGame;

class GameWorld
{
protected:
	GameObjectManager *objectManager;
	OGLFirstPersonCamera *camera;
	PCInputSystem *inputSystem;
	TheGame* theGame;

public:
	GameWorld(GameObjectManager *objectManager, OGLFirstPersonCamera *camera);

	virtual ~GameWorld();

	GameObjectManager *getObjectManager() { return this->objectManager; }

	OGLFirstPersonCamera *getCamera() { return this->camera; }

	void setTheGame(TheGame* theGame) { this->theGame = theGame; }

	void setInputSystem(PCInputSystem* inputSystem) { this->inputSystem = inputSystem; }

	virtual void update(float elapsedSeconds);

	virtual void processInputs();	
};

#endif

