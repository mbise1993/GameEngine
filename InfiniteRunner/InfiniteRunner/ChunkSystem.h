#pragma once
#ifndef GAME_CHUNKSYSTEM_H
#define GAME_CHUNKSYSTEM_H

#include "ChunkRow.h"
#include "Chunk.h"
#include "ChunkGenerator.h"
#include "EngineTypes.h"

#include <glm\glm.hpp>

class Logger;
class OGLObject;

struct ChunkSettings
{
public:
	/*
	The number of chunks to place on either side of the center chunk.
	For example a radius of 1 results in 3 chunks total: Center chunk, plus one on each side.
	*/
	int rowRadius;

	/*
	Size of a single chunk.
	X & Z dimensions.
	*/
	float chunkDimensions;

	/*
	The maximum distance to render chunks.
	*/
	float renderDistance_zFar;

	/*
	Determines how densly to populate the chunks.
	A value of 0.0 = none;
	A value of 1.0 = every chunk;
	*/
	float generationDensity;
};

class ChunkSystem
{
private:
	/*
	The logger.
	*/
	Logger* logger;

	/*
	Chunk settings.
	Chunk system owns.
	*/
	ChunkSettings* settings;

	/*
	Array of rows. Used as a circular queue.
	*/
	ChunkRow** chunkRows;

	/*
	The number of rows.
	*/
	short rowCount;

	/*
	Index of the first row.
	This is the row closest to the camera.
	*/
	short firstRowIndex;

	/*
	Tracks the debug plane if one has been set.
	*/
	OGL3DObject* debugPlane;

	/*
	Populates chunks.
	*/
	ChunkGenerator *generator;

	ChunkCoord lastPlayerCoord;

public:
	/*
	Creates the chunk system using the specified settings.
	*/
	ChunkSystem( ChunkSettings* settings, Logger* logger );

	/*
	Destructor.
	*/
	virtual ~ChunkSystem();

	/*
	Converts a position to chunk coords.
	*/
	ChunkCoord convertPosition( glm::vec3 position );

	/*
	Updates all active/visible chunks.
	*/
	void updateChunks( glm::vec3 playerPosition, float elapsedSeconds );

	/*
	Checks if the enity has collided with anything in the chunk system.
	Returns true if a collision has occured.
	*/
	bool checkCollision( OGL3DObject* entity );

	/*
	Renders all active/visible chunks.
	*/
	void renderChunks();

	/*
	Sets the chunk generator.
	*/
	void setGenerator( ChunkGenerator* generator );

	void setDebugPlane( OGL3DObject* plane );

private:
	/*
	Handles circular queue math.
	*/
	inline ChunkRow *getRow( short index );
};

#endif
