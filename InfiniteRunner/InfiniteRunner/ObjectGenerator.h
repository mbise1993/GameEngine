#pragma once
#ifndef OBJECT_GENERATOR
#define OBJECT_GENERATOR

#include "RGBA.h"
#include "EngineTypes.h"

#include <map>
#include <vector>


class OGL3DObject;

struct ElementArray
{
	float *vertexData;
	index_buffer_t *indexData;
};

struct Position
{
	float x, y, z, w;
};

struct Color
{
	float r, g, b, a;
};

struct Normal
{
	float x, y, z;
};

struct Tex
{
	float s, t;
};

struct Triangle
{
	uint idx1, idx2, idx3;
};

struct Line
{
	uint idx1, idx2;
};

class ObjectGenerator
{
public:
	std::map<uint, Position> positions;
	std::map<uint, Color> colors;
	std::map<uint, Normal> normals;
	std::map<uint, Tex> textureCoords;
	std::vector<Triangle> triangles;
	std::vector<Line> lines;

public:
	ObjectGenerator();
	~ObjectGenerator();

public:
	void clear();

	float* generateVertexData();

	index_buffer_t* generateIndexData();

	uint getNumberOfVertices();

	static float* generateXZSurface( float width = 1, float depth = 1 );

	static float* generatePyramid( float width = 1, float depth = 1, float height = 1 );

	static ElementArray generatePyramidIndexedArray(
		float width = 1, float depth = 1, float height = 1,
		RGBA faceColor = { 1, 1, 1, 1 } );

	static ElementArray generateLineBoxIndexedArray(
		float width = 1, float depth = 1, float height = 1,
		RGBA lineColor = { 1, 1, 1, 1 } );

	static  ElementArray generateFlatSurface(
		int numberOfWidthSegments = 10,
		int numberOfDepthSegments = 10,
		float width = 10, float depth = 10,
		RGBA surfaceColor = { 1, 1, 1, 1 } );

	static  ElementArray generateTextureFlatSurface(
		int numberOfWidthSegments = 10,
		int numberOfDepthSegments = 10,
		float width = 10, float depth = 10,
		RGBA surfaceColor = { 1, 1, 1, 1 } );

	static ElementArray generateCuboid(
		float width = 1, float depth = 1, float height = 1,
		RGBA faceColor = { 1, 1, 1, 1 } );

	// Generates the vertex attributes (position, color, and normal) and indexes
	// for a sphere on the heap.  The first element in both arrays is the number
	// of values.  Vertices are stored first, then colors, and then normals.
	// Params
	//    radius - the radius of the sphere
	//    slices - the number of longitudinal elements
	//    stacks - the number of latitudinal elements
	//    surfaceColor - the color of the sphere
	// Return values:
	// - ElementArray::vertexData = pointer to the vertex data
	// - ElementArray::indexData = pointer to the index data
	static ElementArray generateSphere(
		float radius = 1, int slices = 5, int stacks = 5, RGBA surfaceColor = { 1, 1, 1, 1 } );

protected:
	uint getSizeOfVextexData();

	uint getSizeOfIndexData();

	uint getComponentCount();



};

#endif

