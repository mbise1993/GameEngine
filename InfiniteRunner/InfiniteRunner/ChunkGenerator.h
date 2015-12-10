#pragma once
#ifndef GAME_CHUNK_GENERATOR_H
#define GAME_CHUNK_GENERATOR_H

#include "Chunk.h"
#include "OGL3DObject.h"

#include <random>
#include <vector>


class ChunkGenerator
{
private:
	/*
	Random number generator.
	*/
	std::default_random_engine randomEngine;

	/*
	Objects that can be used to populate chunks.
	*/
	std::vector<OGL3DObject*> obstaclePool;

	/*
	Number of objects in the pool.
	*/
	int poolSize;

	/*
	Half the size of a chunk.
	*/
	float offsetRadius;

	/*
	The % chance to place something in a chunk.
	*/
	float populateChance;

public:
	ChunkGenerator( ChunkSettings* settings );

	virtual ~ChunkGenerator();

	void addObjectToPool( OGL3DObject *object );

	void populateChunk( Chunk *chunk );

	void updatePoolObjects( float elapsedSeconds );

private:
	/*
	Generates a random number float.
	*/
	float rndFloat( float min = 0.0f, float max = 1.0f );

	/*
	Generates a random number integer.
	*/
	int rndInt( int min = 0, int max = 100 );

	/*
	Returns true if a random number is <= the specified percentage.
	*/
	bool percentChance( float percent );
};
#endif
