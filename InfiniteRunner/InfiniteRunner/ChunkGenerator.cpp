#include "ChunkGenerator.h"
#include "ChunkSystem.h"
#include "OGL3DObject.h"

using std::vector;

ChunkGenerator::ChunkGenerator( ChunkSettings *settings )
{
	// Set the seed to a constant value
	// This will cause the game to produce the same chunks every run
	// TODO: When a reset event is thrown this needs to be reset.
	this->randomEngine.seed( 123 );

	// Set poolsize
	this->poolSize = 0;

	// Calculate offset radius
	this->offsetRadius = settings->chunkDimensions / 2.0f;

	// Set chance
	this->populateChance = settings->generationDensity;
}

ChunkGenerator::~ChunkGenerator()
{}

void ChunkGenerator::addObjectToPool( OGL3DObject * object )
{
	// Push to back of pool
	this->obstaclePool.emplace_back( object );
	this->poolSize = this->obstaclePool.size() - 1;
}

void ChunkGenerator::populateChunk( Chunk * chunk )
{
	if ( this->poolSize < 0 ) return;

	// Setup for no object
	OGL3DObject *object = nullptr;
	float offX = 0.0f;
	float offY = 0.0f;
	float offZ = 0.0f;

	if ( this->percentChance( this->populateChance ) )
	{
		// Select object
		object = this->obstaclePool[this->rndInt( 0, poolSize )];

		// Select offsets
		offX = this->rndFloat( -this->offsetRadius, offsetRadius );
		offY = this->rndFloat( -this->offsetRadius / 3.0f, this->offsetRadius / 3.0f );
		offZ = this->rndFloat( -this->offsetRadius, offsetRadius );
	}

	// Populate chunk
	chunk->setOccupant( object, offX, offY, offZ );
}

void ChunkGenerator::updatePoolObjects( float elapsedSeconds )
{
	for ( int i = 0; i <= this->poolSize; ++i )
	{
		this->obstaclePool[i]->update( elapsedSeconds );
	}
}

float ChunkGenerator::rndFloat( float min, float max )
{
	std::uniform_real_distribution<float> distribution( min, max );
	return distribution( this->randomEngine );
}

int ChunkGenerator::rndInt( int min, int max )
{
	std::uniform_int_distribution<int> distribution( min, max );
	return distribution( this->randomEngine );
}

bool ChunkGenerator::percentChance( float percent )
{
	float v = this->rndFloat( 0.0f, 1.0f );
	return (v <= percent);
}
