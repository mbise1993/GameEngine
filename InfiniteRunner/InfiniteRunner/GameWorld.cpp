#include <Windows.h>
#include "GameWorld.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "PCInputSystem.h"
#include "LightSource.h"
#include "TheGame.h"
#include "OGLCamera.h"
#include "Logger.h"

GameWorld::GameWorld( GameObjectManager *objectManager, Camera *camera, Logger* logger ) : BaseObject( logger )
{
	this->objectManager = objectManager;
	this->camera = camera;
	this->inputSystem = nullptr;
}

GameWorld::~GameWorld()
{
	delete this->objectManager;
	delete this->camera;
	delete this->skybox;
	for ( size_t i = 0; i < this->localLights.size(); i++ )
	{
		delete this->localLights[i];
	}
}

void GameWorld::updateSkybox()
{
	OGLCamera* oglCamera = (OGLCamera*)this->camera;
	glm::vec3 camPos = oglCamera->getPosition();

	// TODO: Why is this correction needed?
	float correction = 1.0151f;

	this->skybox->referenceFrame.setPosition( camPos.x, 0, camPos.z * correction );
}

void GameWorld::update( float elapsedSeconds )
{
	for ( size_t i = 0; i < this->localLights.size(); i++ )
	{
		this->localLights[i]->update( elapsedSeconds );
	}
	this->objectManager->update( elapsedSeconds );
	this->objectManager->updateVisibleObjects();
}

void GameWorld::processInputs()
{
	this->theGame->processInputs();
}
