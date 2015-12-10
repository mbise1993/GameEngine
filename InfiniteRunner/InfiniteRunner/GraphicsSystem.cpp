#include "GraphicsSystem.h"
#include "ShaderManager.h"
#include "GameWorld.h"
#include "ViewingFrustum.h"
#include "Logger.h"
#include "Camera.h"
#include "TextureManager.h"

#include <cmath>

GraphicsSystem::GraphicsSystem(
	ShaderManager *shaderManager,
	GameWorld *gameWorld,
	ViewingFrustum *frustum,
	TextureManager *textureManager ) : BaseObject( nullptr )
{
	this->shaderManager = shaderManager;
	this->gameWorld = gameWorld;
	this->frustum = frustum;
	this->textureManager = textureManager;
	this->setFrameRate( 30 );
	this->totalElapsedSeconds = 0;
}

GraphicsSystem::~GraphicsSystem()
{
	delete this->shaderManager;
	delete this->gameWorld;
	delete this->frustum;
	delete this->textureManager;
}

void GraphicsSystem::setLogger( Logger* logger )
{
	this->logger = logger;
	this->shaderManager->setLogger( this->logger );
}

void GraphicsSystem::update( float elapsedSeconds )
{
	const int MAX_MISSED_TICKS = 10;

	this->gameWorld->processInputs();

	// TODO: FPS tacking
	//this->logger->debug( "FPS: " + std::to_string( 1.0f / elapsedSeconds ) );

	this->totalElapsedSeconds += elapsedSeconds;
	unsigned int tickCount = 0;

	// If this->totalElapsedSeconds < tickLengthSeconds then 0 ticks need to be updated
	// If this->totalElapsedSeconds = tickLengthSeconds then 1 tick needs to be updated (and so forth).
	// If tickCount is large, then either your game was asleep, or the machine cannot keep up.
	if ( this->totalElapsedSeconds >= this->tickLengthSeconds )
	{
		tickCount = (unsigned int)floorf( this->totalElapsedSeconds / this->tickLengthSeconds );
		this->totalElapsedSeconds = 0;
	}

	// Tick skip warning
	if ( tickCount > MAX_MISSED_TICKS )
	{
		this->logger->debug( "Skipping " + std::to_string( tickCount - MAX_MISSED_TICKS ) + " missed ticks." );
		tickCount = MAX_MISSED_TICKS;
	}

	for ( unsigned int tick = 0; tick < tickCount; tick++ )
	{
		this->gameWorld->update( this->tickLengthSeconds );
	}

	this->gameWorld->getCamera()->update( elapsedSeconds );
	this->gameWorld->updateSkybox();
}

void GraphicsSystem::setFrameRate( unsigned int frameRate )
{
	this->tickLengthSeconds = 1.0f / frameRate;
}


