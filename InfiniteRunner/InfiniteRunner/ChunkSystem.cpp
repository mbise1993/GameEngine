#include "ChunkSystem.h"
#include "Logger.h"
#include "OGL3DObject.h"

#include <cmath>

ChunkSystem::ChunkSystem( ChunkSettings* settings, Logger* logger )
{
	// Set the logger
	this->logger = logger;

	// Store the settings
	this->settings = settings;

	// Calculate the number of rows
	this->rowCount = 2 + (short)((settings->renderDistance_zFar / settings->chunkDimensions) + 0.5f);

	// Create the rows
	this->chunkRows = new ChunkRow*[this->rowCount];
	for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
	{
		this->chunkRows[rowIdx] = new ChunkRow( this->settings, rowIdx );
	}

	// Set the first row to 0
	this->firstRowIndex = 0;

	this->lastPlayerCoord.x = 0;
	this->lastPlayerCoord.z = 0;

	this->generator = nullptr;
	this->debugPlane = nullptr;

}

ChunkSystem::~ChunkSystem()
{
	// Delete the rows
	for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
	{
		delete this->chunkRows[rowIdx];
	}
	delete[] this->chunkRows;

	// Delete settings
	delete this->settings;

	// Delete generator
	if ( this->generator != nullptr ) delete this->generator;

	if ( this->debugPlane != nullptr ) delete this->debugPlane;
}

ChunkCoord ChunkSystem::convertPosition( glm::vec3 position )
{
	return ChunkCoord(
		(int)(position.x / this->settings->chunkDimensions),
		(int)(position.z / this->settings->chunkDimensions) );
}

void ChunkSystem::updateChunks( glm::vec3 playerPosition, float elapsedSeconds )
{
	// Get the player coords
	ChunkCoord playerCoord = this->convertPosition( playerPosition );

	// Compare against last
	if ( (this->lastPlayerCoord.x != playerCoord.x)
		|| (this->lastPlayerCoord.z != playerCoord.z) )
	{

		// Set last coord
		this->lastPlayerCoord = playerCoord;

		// Subtract 1, keeping the row, just behind the player, active
		playerCoord.z--;

		short rowWithPlayer = 0;

		// Move through each row, starting at the first row, ending in the row with the player in it
		for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
		{
			// Get the row
			ChunkRow *row = this->getRow( this->firstRowIndex + rowIdx );

			// Is the player in this row?
			if ( row->centerCoordinate.z == playerCoord.z )
			{
				// Found the row with the player in it
				rowWithPlayer = (this->firstRowIndex + rowIdx) % this->rowCount;
				break;
			}
			else
			{
				// Mark for regenerate
				row->needsRegenerate = true;
			}
		}

		// Set first row to the row where the player was found (or 0 if the player moved so far it skipped all rows)
		this->firstRowIndex = rowWithPlayer;

		// Regenerate and update rows
		for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
		{
			// Get the row
			ChunkRow *row = this->getRow( this->firstRowIndex + rowIdx );
			if ( row->needsRegenerate )
			{
				// Regenerate
				row->regenerateRow( playerCoord.z + rowIdx );
			}

			// Update the row
			row->updateRow( playerCoord );
		}
	}

	// Update all pool objects
	if ( this->generator != nullptr )
	{
		this->generator->updatePoolObjects( elapsedSeconds );
	}
}

bool ChunkSystem::checkCollision( OGL3DObject * entity )
{
	// Update the bounding box
	entity->boundingBox.referenceFrame = entity->referenceFrame;
	entity->boundingBox.applyTransform();

	// Calculate coord of the entity
	ChunkCoord entityCoord = this->convertPosition( entity->referenceFrame.getPosition() );

	// Get the row the entity is in
	ChunkRow *row = this->getRow( entityCoord.z );

	bool didCollide = false;

	for ( int i = -1; (i <= 1) && (!didCollide); ++i )
	{
		// Get the chunk
		Chunk *chunk = row->getChunk( entityCoord.x + i );

		// Check for collision
		didCollide = chunk->checkOccupantCollision( entity );
	}

	return didCollide;
}

void ChunkSystem::renderChunks()
{
	for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
	{
		this->chunkRows[rowIdx]->renderRow();
	}
}

void ChunkSystem::setGenerator( ChunkGenerator * generator )
{
	this->generator = generator;
	for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
	{
		this->chunkRows[rowIdx]->setGenerator( this->generator );
	}
}

void ChunkSystem::setDebugPlane( OGL3DObject * plane )
{
	this->debugPlane = plane;

	for ( int rowIdx = 0; rowIdx < this->rowCount; ++rowIdx )
	{
		this->chunkRows[rowIdx]->setDebugPlane( plane );
	}
}

inline ChunkRow * ChunkSystem::getRow( short index )
{
	short idx = index % this->rowCount;
	if ( idx < 0 ) idx += this->rowCount;
	return this->chunkRows[idx];
}
