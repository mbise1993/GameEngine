#pragma once
#ifndef GAME_CHUNKROW_H
#define GAME_CHUNKROW_H

#include "Chunk.h"

class ChunkGenerator;

class ChunkRow
{
private:
	/*
	The number of chunks in this row
	*/
	int chunkCount;

	/*
	Number of chunks beside the center chunk.
	*/
	int chunkRadius;

	/*
	Array of chunks.
	*/
	Chunk** chunks;

public:
	ChunkCoord centerCoordinate;
	bool needsRegenerate;

public:
	ChunkRow( ChunkSettings* settings, int ZCoord );
	virtual ~ChunkRow();

	void renderRow();

	void regenerateRow( int ZCoord );

	void setDebugPlane( OGL3DObject* plane );

	void updateRow( ChunkCoord playerCoord );

	/*
	Sets the chunk generator for this row.
	*/
	void setGenerator( ChunkGenerator* generator );

	/*
	Gets the chunk at the specified X coordinate.
	*/
	Chunk* getChunk( int XCoord );

private:
	inline short getChunkIdX( int xCoord );
};

#endif
