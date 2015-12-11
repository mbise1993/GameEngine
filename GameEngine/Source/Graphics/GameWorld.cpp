#include <Windows.h>
#include "GameWorld.h"
#include "Objects/GameObjectManager.h"
#include "PCInputSystem.h"
#include "Cameras/OGLFirstPersonCamera.h"
#include "TheGame.h"

GameWorld::GameWorld(
	GameObjectManager *objectManager, OGLFirstPersonCamera *camera)
{
	this->objectManager = objectManager;
	this->camera = camera;
	this->inputSystem = NULL;
}

GameWorld::~GameWorld()
{
	delete this->objectManager;
	delete this->camera;
}

void GameWorld::update(float elapsedSeconds)
{
	this->objectManager->update(this->camera->getPosition(), elapsedSeconds);
	this->objectManager->updateVisibleObjects();
}

void GameWorld::processInputs()
{
	this->theGame->processInputs();
}
