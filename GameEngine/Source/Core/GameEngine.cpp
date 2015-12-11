#include "GameEngine.h"
#include "Logger.h"
#include "Shaders/OGLShaderManager.h"
#include "GameWindow.h"
#include "GraphicsSystem.h"
#include "CoreSystem.h"
#include "PCInputSystem.h"
#include "GameWorld.h"
#include "ITimer.h"
#include "Objects/GameObjectManager.h"
#include "Objects/OGLObject.h"
#include "TheGame.h"

GameEngine::GameEngine(
	Logger *logger,
	TheGame* theGame,
	CoreSystem *core, 
	GraphicsSystem *graphics, 
	GameWindow *window,
	PCInputSystem *inputSystem,
	ITimer *timer)
{
	this->logger = logger;

	this->core = core;
	this->core->setLogger(this->logger);

	this->theGame = theGame;
	this->theGame->gameEngine = this;

	this->graphics = graphics;
	this->graphics->setTheGame(this->theGame);
	this->graphics->setLogger(this->logger);

	this->window = window;
	this->window->setLogger(this->logger);
	this->window->setGameEngine(this);

	this->inputSystem = inputSystem;
	this->graphics->getGameWorld()->setInputSystem(this->inputSystem);
	this->graphics->getGameWorld()->setTheGame(this->theGame);

	this->timer = timer;

	this->initialized = false;
}

GameEngine::~GameEngine()
{
	delete this->logger;
	delete this->theGame;
	delete this->core;
	delete this->graphics;
	delete this->window;
	delete this->inputSystem;
	delete this->timer;
}

void GameEngine::initializeWindowAndGraphics()
{
	// First create the window
	this->initialized = this->window->create();
	if (this->initialized) {
		// Next, intialize the graphics system
		this->initialized = this->graphics->initialize();
		// Set up the GPU
		if (this->initialized) {
			setupGraphicsParameters();
			this->graphics->setUpViewingEnvironment();
		}
	}
}

void GameEngine::loadShaders()
{
	this->theGame->loadShaders();
}

void GameEngine::setupGraphicsParameters()
{
	this->theGame->setupGraphicsParameters();
}

void GameEngine::setupGame(ObjectLoader* loader)
{
	this->theGame->setup(loader);
}

void GameEngine::run()
{
	if (this->initialized){
		this->window->show();
		this->window->listenForEvents(this->timer);
	}
	else {
		if (this->logger) {
			this->logger->log("The engine was not initialized!");
		}
	}
}


