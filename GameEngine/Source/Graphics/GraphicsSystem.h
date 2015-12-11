#pragma once
#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "RGBA.h"

class ShaderManager;
class Logger;
class GameWorld;
class ViewingFrustum;
class TheGame;

class GraphicsSystem
{
protected:
	Logger *logger;
	ShaderManager *shaderManager;
	GameWorld *gameWorld;
	ViewingFrustum *frustum;
	TheGame* theGame;
	bool initialized;
	float tickLengthSeconds;
	float totalElapsedSeconds;

public:
	GraphicsSystem(
		ShaderManager *shaderManager, 
		GameWorld *gameWorld, 
		ViewingFrustum *frustum);

	virtual ~GraphicsSystem();

	virtual void setLogger(Logger* logger);

	virtual void setTheGame(TheGame* theGame) { this->theGame = theGame; }

	virtual ShaderManager *getShaderManager() { return this->shaderManager; }

	virtual GameWorld *getGameWorld() { return this->gameWorld; }

	virtual ViewingFrustum *getViewingFrustum() { return this->frustum; }

	virtual bool initialize() = 0;

	virtual void setUpViewingEnvironment() = 0;

	virtual void clearViewPort(
		unsigned int left, unsigned int top, 
		unsigned int width, unsigned int height, 
		RGBA color) = 0;

	virtual void update(float elapsedSeconds);

	virtual void render() = 0;

	virtual void sendViewingParametersToGPU() {}

	bool isInitialized() const { return this->initialized; }

	// Param:
	// frameRate - the number of frames per second e.g. 50
	void setFrameRate(unsigned int frameRate);

	void setLastTickSeconds(float lastTickSeconds) { 
		this->totalElapsedSeconds = lastTickSeconds; 
	}
};

#endif

