#include "ChunkRow.h"
#include "ChunkSystem.h"


#include "WindowsConsoleLogger.h"


ChunkRow::ChunkRow( ChunkSettings * settings, int ZCoord )
{
	// Set the radius
	this->chunkRadius = settings->rowRadius;

	// Set the z coordinate
	this->centerCoordinate.z = ZCoord;

	// Calculate the number of chunks in the row
	this->chunkCount = 1 + (this->chunkRadius * 2);

	// Generate the chunks
	this->chunks = new Chunk*[this->chunkCount];
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		this->chunks[chunkIdx] = new Chunk( settings, chunkIdx - this->chunkRadius, ZCoord );
	}
}

ChunkRow::~ChunkRow()
{
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		delete this->chunks[chunkIdx];
	}
	delete[] this->chunks;
}

void ChunkRow::renderRow()
{
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		this->chunks[chunkIdx]->render();
	}
}

void ChunkRow::regenerateRow( int ZCoord )
{
	// Set the z coordinate
	this->centerCoordinate.z = ZCoord;

	// Regen chunks
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		this->chunks[chunkIdx]->regenChunk( ZCoord );
	}

	// Mark no longer needs regen
	this->needsRegenerate = false;
}

void ChunkRow::setDebugPlane( OGL3DObject * plane )
{
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		this->chunks[chunkIdx]->setGenerator( nullptr );
		this->chunks[chunkIdx]->setDebugPlane( plane );
	}
}

void ChunkRow::updateRow( ChunkCoord playerCoord )
{
	// Check each chunk to see if it's X needs to be updated
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		// Calculate the chunks relative position to the chunk with the player in it
		int relativeX = chunkIdx - this->chunkRadius;

		// Calculate the chunks X position
		int newX = playerCoord.x + relativeX;

		// Get the chunk
		Chunk *chunk = this->getChunk( newX );

		// Does the chunk need to be moved?
		if ( chunk->coordinate.x != newX )
		{
			chunk->updateXCoord( newX );
		}
	}
}

void ChunkRow::setGenerator( ChunkGenerator * generator )
{
	for ( int chunkIdx = 0; chunkIdx < this->chunkCount; ++chunkIdx )
	{
		this->chunks[chunkIdx]->setGenerator( generator );
	}
}

Chunk * ChunkRow::getChunk( int XCoord )
{
	return this->chunks[ this->getChunkIdX( XCoord ) ];
}

inline short ChunkRow::getChunkIdX( int xCoord )
{
	short idx = (this->chunkRadius + xCoord) % this->chunkCount;
	if ( idx < 0 ) idx += this->chunkCount;
	return idx;
}
