#pragma once
#ifndef GAME_CHUNK_H
#define GAME_CHUNK_H

#include "OGLReferenceFrame.h"

#include <glm\glm.hpp>

struct ChunkSettings;
class OGL3DObject;
class OGLReferenceFrame;
class ChunkGenerator;

/*
Coordinate on the chunk grid.
*/
struct ChunkCoord
{
	int x;
	int z;

	ChunkCoord( int x, int z )
	{
		this->x = x;
		this->z = z;
	}

	ChunkCoord()
	{
		this->x = 0;
		this->z = 0;
	}
};

class Chunk
{
private:
	/*
	X & Z size.
	*/
	float dimensions;

	OGLReferenceFrame chunkFrame;

	ChunkGenerator *generator;

public:
	ChunkCoord coordinate;

	OGL3DObject *occupant;
	OGLReferenceFrame occupantOffset;

public:
	Chunk( ChunkSettings *settings, int XCoord, int ZCoord );
	virtual ~Chunk();

	/*
	Regenerates the chunk.
	*/
	void regenChunk( int ZCoord );

	/*
	Renders the chunk.
	*/
	void render();

	/*
	Sets the object in this chunk.
	*/
	void setOccupant( OGL3DObject *occupant, float xOffset, float yOffset, float zOffset );

	/*
	Sets the generator that populates this chunk.
	*/
	void setGenerator( ChunkGenerator *generator );

	/*
	Moves the chunk to a new X position in the row.
	*/
	void updateXCoord( int XCoord );

	/*
	Checks if the occupant of this chunk is colliding with the entity.
	*/
	bool checkOccupantCollision( OGL3DObject* entity );

	void setDebugPlane( OGL3DObject *plane );

private:
	void calculatePosition();

	inline glm::mat4 getRenderOrientation();
};

#endif
