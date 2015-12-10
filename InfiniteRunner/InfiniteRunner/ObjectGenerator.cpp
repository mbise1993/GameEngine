#include "ObjectGenerator.h"
#include "MathUtil.h"
#include "OGL3DObject.h"

using std::vector;
using std::map;

ObjectGenerator::ObjectGenerator()
{}

ObjectGenerator::~ObjectGenerator()
{}

void ObjectGenerator::clear()
{
	this->positions.clear();
	this->colors.clear();
	this->normals.clear();
	this->textureCoords.clear();
	this->triangles.clear();
	this->lines.clear();
}

float* ObjectGenerator::generateVertexData()
{
	unsigned int sizeOfVertexData = this->getSizeOfVextexData();
	float* vertexData = new float[sizeOfVertexData + 1];
	vertexData[0] = (float)sizeOfVertexData;

	unsigned int positionComponentCount = sizeof( Position ) / sizeof( float );
	unsigned int colorComponentCount = sizeof( Color ) / sizeof( float );
	unsigned int normalComponentCount = sizeof( Normal ) / sizeof( float );
	unsigned int textureComponentCount = sizeof( Tex ) / sizeof( float );
	unsigned int numberOfVertices = this->getNumberOfVertices();
	unsigned int pi = 1;
	unsigned int ci = (positionComponentCount * numberOfVertices) + 1;
	unsigned int ni = ((positionComponentCount + colorComponentCount) * numberOfVertices) + 1;
	unsigned int ti = ((positionComponentCount + colorComponentCount + normalComponentCount)
		* numberOfVertices) + 1;
	for ( size_t i = 0; i < numberOfVertices; i++ )
	{
		if ( this->positions.size() > 0 )
		{
			vertexData[pi++] = this->positions[i].x;
			vertexData[pi++] = this->positions[i].y;
			vertexData[pi++] = this->positions[i].z;
			vertexData[pi++] = this->positions[i].w;
		}
		if ( this->colors.size() > 0 )
		{
			vertexData[ci++] = this->colors[i].r;
			vertexData[ci++] = this->colors[i].g;
			vertexData[ci++] = this->colors[i].b;
			vertexData[ci++] = this->colors[i].a;
		}
		if ( this->normals.size() > 0 )
		{
			vertexData[ni++] = this->normals[i].x;
			vertexData[ni++] = this->normals[i].y;
			vertexData[ni++] = this->normals[i].z;
		}
		if ( this->textureCoords.size() > 0 )
		{
			vertexData[ti++] = this->textureCoords[i].s;
			vertexData[ti++] = this->textureCoords[i].t;
		}
	}
	return vertexData;
}

index_buffer_t* ObjectGenerator::generateIndexData()
{
	unsigned int sizeOfIndexData = this->getSizeOfIndexData();
	index_buffer_t* indexData = new index_buffer_t[sizeOfIndexData + 1];
	indexData[0] = (index_buffer_t)sizeOfIndexData;

	unsigned int count = 0;
	if ( this->lines.size() > 0 ) count = this->lines.size();
	else if ( this->triangles.size() > 0 ) count = this->triangles.size();
	unsigned int idx = 1;
	for ( size_t i = 0; i < count; i++ )
	{
		if ( this->lines.size() > 0 )
		{
			indexData[idx++] = this->lines[i].idx1;
			indexData[idx++] = this->lines[i].idx2;
		}
		else if ( this->triangles.size() > 0 )
		{
			indexData[idx++] = this->triangles[i].idx1;
			indexData[idx++] = this->triangles[i].idx2;
			indexData[idx++] = this->triangles[i].idx3;
		}
	}
	return indexData;
}

unsigned int ObjectGenerator::getSizeOfVextexData()
{
	unsigned int numberOfElements = 0;
	numberOfElements += this->positions.size() * (sizeof( Position ) / sizeof( float ));
	numberOfElements += this->colors.size() * (sizeof( Color ) / sizeof( float ));
	numberOfElements += this->normals.size() * (sizeof( Normal ) / sizeof( float ));
	numberOfElements += this->textureCoords.size() * (sizeof( Tex ) / sizeof( float ));
	return numberOfElements;
}

unsigned int ObjectGenerator::getSizeOfIndexData()
{
	unsigned int numberOfElements = 0;
	if ( this->lines.size() > 0 )
	{
		numberOfElements = this->lines.size() * (sizeof( Line ) / sizeof( unsigned int ));
	}
	else if ( this->triangles.size() > 0 )
	{
		numberOfElements = this->triangles.size() * (sizeof( Triangle ) / sizeof( unsigned int ));
	}
	return numberOfElements;
}

unsigned int ObjectGenerator::getComponentCount()
{
	unsigned int componentCount = 0;
	if ( this->positions.size() > 0 )
	{
		componentCount += (sizeof( Position ) / sizeof( float ));
	}
	if ( this->colors.size() > 0 )
	{
		componentCount += (sizeof( Color ) / sizeof( float ));
	}
	if ( this->normals.size() > 0 )
	{
		componentCount += (sizeof( Normal ) / sizeof( float ));
	}
	if ( this->textureCoords.size() > 0 )
	{
		componentCount += (sizeof( Tex ) / sizeof( float ));
	}
	return componentCount;
}

unsigned int ObjectGenerator::getNumberOfVertices()
{
	return this->getSizeOfVextexData() / this->getComponentCount();
}

float* ObjectGenerator::generateXZSurface( float width, float depth )
{
	// This object's origin is in the center.
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;

	//         width
	//          D
	//      { A +--+ F
	// depth{   |\ |
	//      {   | \|
	//      { B +--+ C
	//             E

	// 1 - size + (2 triangles * 3 vertices each * (4 position + 4 color))
	// 1 + (2 * 3 * (4 + 4)) = 49
	float* data = new float[49];

	// The first element stores the number of values
	data[0] = 48;

	int i = 1; // index
			   // Positions
			   // A
	data[i++] = -halfWidth; // x
	data[i++] = 0.0f;       // y
	data[i++] = -halfDepth; // z
	data[i++] = 1.0f;       // w
							// B
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// C
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// D
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// E
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// F
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;

	// Colors
	// A
	data[i++] = 0.0f; // red
	data[i++] = 0.4f; // green
	data[i++] = 0.0f; // blue
	data[i++] = 1.0f; // alpha
					  // B
	data[i++] = 0.0f;
	data[i++] = 0.4f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// C
	data[i++] = 0.0f;
	data[i++] = 0.4f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// D
	data[i++] = 0.0f;
	data[i++] = 0.4f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// E
	data[i++] = 0.0f;
	data[i++] = 0.4f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// F
	data[i++] = 0.0f;
	data[i++] = 0.4f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	return data;
}

float* ObjectGenerator::generatePyramid( float width, float depth, float height )
{
	// This pyramid's origin is in the center of the base.
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;

	// 1 size + (6 triangles * 3 vertices each * (4 position + 4 color))
	// 1 + (6 * 3 * (4 + 4)) = 145
	float* data = new float[145];

	// The first element stores the number of elements.
	data[0] = 144;

	int i = 1;
	// South face
	// Lower-left
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = height;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// East face
	// Lower-left
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = height;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// North face
	// Lower-left
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = height;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// West face
	// Lower-left
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = height;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// Base
	// Upper-left
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Lower-left
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Upper-left
	data[i++] = -halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = -halfDepth;
	data[i++] = 1.0f;
	// Upper-right
	data[i++] = halfWidth;
	data[i++] = 0.0f;
	data[i++] = halfDepth;
	data[i++] = 1.0f;

	// Colors
	// South face
	// Lower-left
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;

	// East face
	// Lower-left
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// North face
	// Lower-left
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;

	// West face
	// Lower-left
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	// Apex
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;

	// Base
	// Upper-left
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Lower-left
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Upper-left
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Lower-right
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;
	// Upper-right
	data[i++] = 0.0f;
	data[i++] = 0.0f;
	data[i++] = 1.0f;
	data[i++] = 1.0f;

	return data;
}

ElementArray ObjectGenerator::generatePyramidIndexedArray(
	float width, float depth, float height, RGBA faceColor )
{
	// This pyramid's origin is in the center.
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;

	//           width
	//      { 0 +-----+ 3
	//      {   |\   /|
	//      {   | \ / | 
	// depth{   |  +4 |
	//      {   | / \ |
	//      {   |/   \|
	//      { 1 +-----+ 2

	// The pyramid has 5 vertices, each vertex has a 4 position component
	// and a 4 color component.
	// 1 + (5 * (4 + 4)) = 41
	float* vertexData = new float[41];
	// Number of elements
	vertexData[0] = 40;
	int i = 1; // Current index

	// Vertex 0
	vertexData[i++] = -halfWidth;
	vertexData[i++] = 0.0f;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// Vertex 1
	vertexData[i++] = -halfWidth;
	vertexData[i++] = 0.0f;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// Vertex 2
	vertexData[i++] = halfWidth;
	vertexData[i++] = 0.0f;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// Vertex 3
	vertexData[i++] = halfWidth;
	vertexData[i++] = 0.0f;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// Vertex 4
	vertexData[i++] = 0.0f;
	vertexData[i++] = height;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// Colors
	// Vertex 0
	vertexData[i++] = faceColor.red;
	vertexData[i++] = faceColor.green;
	vertexData[i++] = faceColor.blue;
	vertexData[i++] = faceColor.alpha;
	// Vertex 1
	vertexData[i++] = faceColor.red;
	vertexData[i++] = faceColor.green;
	vertexData[i++] = faceColor.blue;
	vertexData[i++] = faceColor.alpha;
	// Vertex 2
	vertexData[i++] = faceColor.red;
	vertexData[i++] = faceColor.green;
	vertexData[i++] = faceColor.blue;
	vertexData[i++] = faceColor.alpha;
	// Vertex 3
	vertexData[i++] = faceColor.red;
	vertexData[i++] = faceColor.green;
	vertexData[i++] = faceColor.blue;
	vertexData[i++] = faceColor.alpha;
	// Vertex 4
	vertexData[i++] = faceColor.red;
	vertexData[i++] = faceColor.green;
	vertexData[i++] = faceColor.blue;
	vertexData[i++] = faceColor.alpha;

	// There are 6 triangles with 3 vertex each.
	// 1 + (6 * 3) = 19
	index_buffer_t* indexData = new index_buffer_t[19];
	indexData[0] = 18;
	i = 1;
	// West side
	indexData[i++] = 0;
	indexData[i++] = 1;
	indexData[i++] = 4;
	// South side
	indexData[i++] = 1;
	indexData[i++] = 2;
	indexData[i++] = 4;
	// East side
	indexData[i++] = 2;
	indexData[i++] = 3;
	indexData[i++] = 4;
	// North side
	indexData[i++] = 3;
	indexData[i++] = 0;
	indexData[i++] = 4;
	// Base 1
	indexData[i++] = 1;
	indexData[i++] = 0;
	indexData[i++] = 3;
	// Base 2
	indexData[i++] = 1;
	indexData[i++] = 3;
	indexData[i++] = 2;

	ElementArray arr = { vertexData, indexData };
	return arr;
}

ElementArray ObjectGenerator::generateLineBoxIndexedArray(
	float width, float depth, float height, RGBA lineColor )
{
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;
	float halfHeight = height / 2.0f;

	// 1 + (8 * (4 + 4)) = 65
	float* vertexData = new float[65];
	vertexData[0] = 64;
	int i = 1;
	// Positions
	// 0
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 1
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 2
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 3
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 4
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 5
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 6
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 7
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;

	// Colors
	// 0
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 1
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 2
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 3
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 4
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 5
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 6
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;
	// 7
	vertexData[i++] = lineColor.red;
	vertexData[i++] = lineColor.green;
	vertexData[i++] = lineColor.blue;
	vertexData[i++] = 1.0f;

	// 1 + (12 * 2) = 25
	index_buffer_t* indexData = new index_buffer_t[25];
	indexData[0] = 24;
	i = 1;
	indexData[i++] = 0;
	indexData[i++] = 1;

	indexData[i++] = 1;
	indexData[i++] = 2;

	indexData[i++] = 2;
	indexData[i++] = 3;

	indexData[i++] = 3;
	indexData[i++] = 0;

	indexData[i++] = 4;
	indexData[i++] = 5;

	indexData[i++] = 5;
	indexData[i++] = 6;

	indexData[i++] = 6;
	indexData[i++] = 7;

	indexData[i++] = 7;
	indexData[i++] = 4;

	indexData[i++] = 0;
	indexData[i++] = 7;

	indexData[i++] = 3;
	indexData[i++] = 4;

	indexData[i++] = 2;
	indexData[i++] = 5;

	indexData[i++] = 1;
	indexData[i++] = 6;

	ElementArray arr = { vertexData, indexData };
	return arr;
}

ElementArray ObjectGenerator::generateFlatSurface( int numberOfWidthSegments, int numberOfDepthSegments, float width, float depth, RGBA surfaceColor )
{
	const int NumberOfFacets = numberOfWidthSegments * numberOfDepthSegments;
	const int NumberOfTriangles = NumberOfFacets * 2;
	const int NumberOfVertices = NumberOfTriangles * 3;
	// 11 = 4 components for position, 4 components for color, and 3 components for the normal
	const int NumberOfComponents = 11;
	int sizeOfVertexData = NumberOfVertices * NumberOfComponents;

	// ---------------------------------------------
	// The Vertex data: position, color, and normals
	// ---------------------------------------------

	float* vertexData = new float[sizeOfVertexData + 1];
	// The first element stores the size of the vertex data
	vertexData[0] = (float)sizeOfVertexData;

	// Positions first
	// ---------------
	// Begin at the back left
	Vector3f A, B, C, D;
	float xPos = -width / 2;
	float zPos = -depth / 2;
	float segmentWidth = width / numberOfWidthSegments;
	float segmentDepth = depth / numberOfDepthSegments;
	int i = 1;
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			A = { xPos, 0.0f, zPos };
			B = { xPos, 0.0f, zPos + segmentDepth };
			C = { xPos + segmentWidth, 0.0f, zPos + segmentDepth };
			D = { xPos + segmentWidth, 0.0f, zPos };
			vertexData[i++] = A.v1;
			vertexData[i++] = A.v2;
			vertexData[i++] = A.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = B.v1;
			vertexData[i++] = B.v2;
			vertexData[i++] = B.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = C.v1;
			vertexData[i++] = C.v2;
			vertexData[i++] = C.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = A.v1;
			vertexData[i++] = A.v2;
			vertexData[i++] = A.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = C.v1;
			vertexData[i++] = C.v2;
			vertexData[i++] = C.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = D.v1;
			vertexData[i++] = D.v2;
			vertexData[i++] = D.v3;
			vertexData[i++] = 1.0f;
			xPos += segmentWidth;
		}
		xPos = -width / 2;
		zPos += segmentDepth;
	}
	// Colors second
	// -------------
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			for ( int count = 0; count < 6; count++ )
			{
				vertexData[i++] = surfaceColor.red;
				vertexData[i++] = surfaceColor.green;
				vertexData[i++] = surfaceColor.blue;
				vertexData[i++] = 1.0f;
			}
		}
	}
	// Normals third
	// -------------
	// Since this is a flat surface, the normals point up the y-axis,
	// otherwise, we'd need to calculate the normal using the cross-product.
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			for ( int count = 0; count < 6; count++ )
			{
				vertexData[i++] = 0.0f;
				vertexData[i++] = 1.0f;
				vertexData[i++] = 0.0f;
			}
		}
	}

	// --------------------------------------
	// The index data: 3 indices per triangle
	// --------------------------------------
	int numberOfIndices = NumberOfTriangles * 3;
	index_buffer_t* indexData = new index_buffer_t[numberOfIndices + 1];
	indexData[0] = numberOfIndices;
	i = 1;
	index_buffer_t index = 0;
	for ( int count = 0; count < NumberOfTriangles; count++ )
	{
		indexData[i++] = index++;
		indexData[i++] = index++;
		indexData[i++] = index++;
	}

	ElementArray arr = { vertexData, indexData };
	return arr;
}

ElementArray ObjectGenerator::generateTextureFlatSurface( int numberOfWidthSegments, int numberOfDepthSegments, float width, float depth, RGBA surfaceColor )
{
	const int NumberOfFacets = numberOfWidthSegments * numberOfDepthSegments;
	const int NumberOfTriangles = NumberOfFacets * 2;
	const int NumberOfVertices = NumberOfTriangles * 3;
	// 13 = 4 components for position, 4 components for color, 3 components for the normal,
	// and 2 for the texture coordinates
	const int NumberOfComponents = 13;
	int sizeOfVertexData = NumberOfVertices * NumberOfComponents;

	// ------------------------------------------------------------------
	// The Vertex data: position, color, normals, and texture coordinates
	// ------------------------------------------------------------------

	float* vertexData = new float[sizeOfVertexData + 1];
	// The first element stores the size of the vertex data
	vertexData[0] = (float)sizeOfVertexData;

	// Positions first
	// ---------------
	// Begin at the back left
	// A--D
	// |\ |
	// | \|
	// B--C
	Vector3f A, B, C, D;
	float xPos = -width / 2;
	float zPos = -depth / 2;
	float segmentWidth = width / numberOfWidthSegments;
	float segmentDepth = depth / numberOfDepthSegments;
	int i = 1;
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			A = { xPos, 0.0f, zPos };
			B = { xPos, 0.0f, zPos + segmentDepth };
			C = { xPos + segmentWidth, 0.0f, zPos + segmentDepth };
			D = { xPos + segmentWidth, 0.0f, zPos };
			vertexData[i++] = A.v1;
			vertexData[i++] = A.v2;
			vertexData[i++] = A.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = B.v1;
			vertexData[i++] = B.v2;
			vertexData[i++] = B.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = C.v1;
			vertexData[i++] = C.v2;
			vertexData[i++] = C.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = A.v1;
			vertexData[i++] = A.v2;
			vertexData[i++] = A.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = C.v1;
			vertexData[i++] = C.v2;
			vertexData[i++] = C.v3;
			vertexData[i++] = 1.0f;

			vertexData[i++] = D.v1;
			vertexData[i++] = D.v2;
			vertexData[i++] = D.v3;
			vertexData[i++] = 1.0f;
			xPos += segmentWidth;
		}
		xPos = -width / 2;
		zPos += segmentDepth;
	}
	// Colors second
	// -------------
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			for ( int count = 0; count < 6; count++ )
			{
				vertexData[i++] = surfaceColor.red;
				vertexData[i++] = surfaceColor.green;
				vertexData[i++] = surfaceColor.blue;
				vertexData[i++] = 1.0f;
			}
		}
	}
	// Normals third
	// -------------
	// Since this is a flat surface, the normals point up the y-axis,
	// otherwise, we'd need to calculate the normal using the cross-product.
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			for ( int count = 0; count < 6; count++ )
			{
				vertexData[i++] = 0.0f;
				vertexData[i++] = 1.0f;
				vertexData[i++] = 0.0f;
			}
		}
	}
	// Texture coordinates last
	// ------------------------
	// A--D
	// |\ |
	// | \|
	// B--C
	for ( int row = 0; row < numberOfDepthSegments; row++ )
	{
		for ( int col = 0; col < numberOfWidthSegments; col++ )
		{
			// A
			vertexData[i++] = 0.0f;
			vertexData[i++] = 1.0f;
			// B
			vertexData[i++] = 0.0f;
			vertexData[i++] = 0.0f;
			// C
			vertexData[i++] = 1.0f;
			vertexData[i++] = 0.0f;
			// A
			vertexData[i++] = 0.0f;
			vertexData[i++] = 1.0f;
			// C
			vertexData[i++] = 1.0f;
			vertexData[i++] = 0.0f;
			// D
			vertexData[i++] = 1.0f;
			vertexData[i++] = 1.0f;
		}
	}

	// --------------------------------------
	// The index data: 3 indices per triangle
	// --------------------------------------
	int numberOfIndices = NumberOfTriangles * 3;
	index_buffer_t* indexData = new index_buffer_t[numberOfIndices + 1];
	indexData[0] = numberOfIndices;
	i = 1;
	index_buffer_t index = 0;
	for ( int count = 0; count < NumberOfTriangles; count++ )
	{
		indexData[i++] = index++;
		indexData[i++] = index++;
		indexData[i++] = index++;
	}

	ElementArray arr = { vertexData, indexData };
	return arr;
}

ElementArray ObjectGenerator::generateCuboid( float width, float depth, float height, RGBA faceColor )
{
	// Since there is now a normal for each vertex and they all point in a different direction
	const int NumberOfFaces = 6;
	int numberOfVertices = NumberOfFaces * 4;
	// 4 components for position, 4 components for color, and 3 components for the normal
	int sizeOfVertexData = numberOfVertices * 11;

	// ---------------------------------------------
	// The Vertex data: position, color, and normals
	// ---------------------------------------------

	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;
	float halfHeight = height / 2.0f;

	float* vertexData = new float[sizeOfVertexData + 1];
	// The first element stores the size of the vertex data
	vertexData[0] = (float)sizeOfVertexData;
	int i = 1;
	// Positions
	// Front face
	// 0
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 1
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 2
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 3
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// Right face
	// 4
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 5
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 6
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 7
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// Back face
	// 8
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 9
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 10
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 11
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// Left face
	// 12
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 13
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 14
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 15
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// Top face
	// 16
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 17
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 18
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 19
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// Bottom face
	// 20
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;
	// 21
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 22
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = -halfDepth;
	vertexData[i++] = 1.0f;
	// 23
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = halfDepth;
	vertexData[i++] = 1.0f;

	// Colors
	for ( int vertex = 0; vertex < numberOfVertices; vertex++ )
	{
		vertexData[i++] = faceColor.red;
		vertexData[i++] = faceColor.green;
		vertexData[i++] = faceColor.blue;
		vertexData[i++] = faceColor.alpha;
	}

	// Normals
	// Front face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = 0;
		vertexData[i++] = 0;
		vertexData[i++] = 1;
	}
	// Right face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = 1;
		vertexData[i++] = 0;
		vertexData[i++] = 0;
	}
	// Back face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = 0;
		vertexData[i++] = 0;
		vertexData[i++] = -1;
	}
	// Left face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = -1;
		vertexData[i++] = 0;
		vertexData[i++] = 0;
	}
	// Top face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = 0;
		vertexData[i++] = 1;
		vertexData[i++] = 0;
	}
	// Bottom face
	for ( int vertex = 0; vertex < 4; vertex++ )
	{
		vertexData[i++] = 0;
		vertexData[i++] = -1;
		vertexData[i++] = 0;
	}

	// --------------------------------------
	// The index data: 3 indices per triangle
	// --------------------------------------
	const int NumberOfTriangles = NumberOfFaces * 2;
	const int NumberOfIndices = NumberOfTriangles * 3;
	index_buffer_t* indexData = new index_buffer_t[NumberOfIndices + 1];
	indexData[0] = (index_buffer_t)NumberOfIndices;
	i = 1;
	// Front face
	indexData[i++] = 0;
	indexData[i++] = 1;
	indexData[i++] = 2;
	indexData[i++] = 0;
	indexData[i++] = 2;
	indexData[i++] = 3;
	// Right face
	indexData[i++] = 4;
	indexData[i++] = 5;
	indexData[i++] = 6;
	indexData[i++] = 4;
	indexData[i++] = 6;
	indexData[i++] = 7;
	// Back face
	indexData[i++] = 8;
	indexData[i++] = 9;
	indexData[i++] = 10;
	indexData[i++] = 8;
	indexData[i++] = 10;
	indexData[i++] = 11;
	// Left face
	indexData[i++] = 12;
	indexData[i++] = 13;
	indexData[i++] = 14;
	indexData[i++] = 12;
	indexData[i++] = 14;
	indexData[i++] = 15;
	// Top face
	indexData[i++] = 16;
	indexData[i++] = 17;
	indexData[i++] = 18;
	indexData[i++] = 16;
	indexData[i++] = 18;
	indexData[i++] = 19;
	// Bottom face
	indexData[i++] = 20;
	indexData[i++] = 21;
	indexData[i++] = 22;
	indexData[i++] = 20;
	indexData[i++] = 22;
	indexData[i++] = 23;

	ElementArray arr = { vertexData, indexData };
	return arr;
}

ElementArray ObjectGenerator::generateSphere( float radius, int slices, int stacks, RGBA surfaceColor )
{
	// See http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
	// Spherical to Cartesian
	// x = r * sin(theta) * sin(phi)
	// y = r * cos(phi)
	// z = r * cos(theta) * sin(phi)

	const int NumberOfVerticesTop = slices * 3;
	const int NumberOfVerticesBottom = slices * 3;
	const int MiddleStackCount = stacks - 2; // Subtract top and bottom stack
	// The middle of the sphere has (stacks - 2) x slices x 6 vertices
	const int NumberOfVerticesMiddle = MiddleStackCount * slices * 6;
	int NumberOfVertices =
		NumberOfVerticesTop + NumberOfVerticesBottom + NumberOfVerticesMiddle;
	// 4 components for position, 4 components for color, and 3 components for the normal
	int sizeOfVertexData = NumberOfVertices * 11;

	float* vertexData = new float[sizeOfVertexData + 1];
	// The first element stores the size of the vertex data
	vertexData[0] = (float)sizeOfVertexData;
	// Position index
	int pi = 1;
	// Color index
	int ci = NumberOfVertices * 4 + 1;
	// Normal index
	int ni = NumberOfVertices * 8 + 1;

	const int NumberOfIndices =
		NumberOfVerticesTop + NumberOfVerticesBottom + NumberOfVerticesMiddle
		+ (NumberOfVerticesMiddle / 2);
	index_buffer_t* indexData = new index_buffer_t[NumberOfIndices + 1];
	indexData[0] = (index_buffer_t)NumberOfIndices;
	int i = 1;
	int idx = 0;

	Vector3f A, B, C, N;
	float stepY = 180.0f / stacks;
	float stepX = 360.0f / slices;
	float phi = stepY;
	float rho = radius;
	float theta, sinTheta, cosTheta, cosPhi, sinPhi, thetaR, phiR;
	// The top pole is composed of triangles.
	//        A
	//       / \
	//      /   \
	//     B-----C
	// Generate top vertices
	for ( theta = 0; theta < 360.0f; theta += stepX )
	{
		// First vertex is the top pole
		A.v1 = 0;
		A.v2 = radius;
		A.v3 = 0;
		vertexData[pi++] = A.v1;
		vertexData[pi++] = A.v2;
		vertexData[pi++] = A.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = A.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;

		// Second vertex
		thetaR = DegToRad( theta );
		phiR = DegToRad( phi );
		sinTheta = sinf( thetaR );
		sinPhi = sinf( phiR );
		cosPhi = cosf( phiR );
		cosTheta = cosf( thetaR );
		B.v1 = rho * sinTheta * sinPhi;
		B.v2 = rho * cosPhi;
		B.v3 = rho * cosTheta * sinPhi;
		vertexData[pi++] = B.v1;
		vertexData[pi++] = B.v2;
		vertexData[pi++] = B.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = B.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;

		// Third vertex
		thetaR = DegToRad( theta + stepX );
		sinTheta = sinf( thetaR );
		cosTheta = cosf( thetaR );
		C.v1 = rho * sinTheta * sinPhi;
		C.v2 = B.v2;
		C.v3 = rho * cosTheta * sinPhi;
		vertexData[pi++] = C.v1;
		vertexData[pi++] = C.v2;
		vertexData[pi++] = C.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = B.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;
	}
	Vector3f A2, C2, D;
	float phiUp = phi;
	float phiDown = phi + stepY;
	//int aIdx, bIdx, cIdx, dIdx, a2Idx, c2Idx;
	// Generate the middle
	for ( int stack = 0; stack < MiddleStackCount; stack++ )
	{
		// Generate 4 vertices for each facet
		// A---D
		// |\  |
		// | \ |
		// |  \|
		// B---C
		//aIdx = idx;
		//bIdx = idx + 1;
		//cIdx = idx + 2;
		//a2Idx = idx + 3;
		//c2Idx = idx + 4;
		//dIdx = idx + 5;
		for ( theta = 0; theta < 360.0f; theta += stepX )
		{
			// A
			thetaR = DegToRad( theta );
			sinTheta = sinf( thetaR );
			cosTheta = cos( thetaR );
			phiR = DegToRad( phiUp );
			sinPhi = sinf( phiR );
			cosPhi = cosf( phiR );
			A.v1 = rho * sinTheta * sinPhi;
			A.v2 = rho * cosPhi;
			A.v3 = rho * cosTheta * sinPhi;
			vertexData[pi++] = A.v1;
			vertexData[pi++] = A.v2;
			vertexData[pi++] = A.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = A.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
			// B
			phiR = DegToRad( phiDown );
			sinPhi = sinf( phiR );
			cosPhi = cosf( phiR );
			B.v1 = rho * sinTheta * sinPhi;
			B.v2 = rho * cosPhi;
			B.v3 = rho * cosTheta * sinPhi;
			vertexData[pi++] = B.v1;
			vertexData[pi++] = B.v2;
			vertexData[pi++] = B.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = B.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
			// C
			thetaR = DegToRad( theta + stepX );
			sinTheta = sinf( thetaR );
			cosTheta = cos( thetaR );
			C.v1 = rho * sinTheta * sinPhi;
			C.v2 = B.v2;
			C.v3 = rho * cosTheta * sinPhi;
			vertexData[pi++] = C.v1;
			vertexData[pi++] = C.v2;
			vertexData[pi++] = C.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = C.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
			// A2
			A2.v1 = A.v1;
			A2.v2 = A.v2;
			A2.v3 = A.v3;
			vertexData[pi++] = A2.v1;
			vertexData[pi++] = A2.v2;
			vertexData[pi++] = A2.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = A2.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
			// C2
			C2.v1 = C.v1;
			C2.v2 = C.v2;
			C2.v3 = C.v3;
			vertexData[pi++] = C2.v1;
			vertexData[pi++] = C2.v2;
			vertexData[pi++] = C2.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = C2.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
			// D
			phiR = DegToRad( phiUp );
			sinPhi = sinf( phiR );
			D.v1 = rho * sinTheta * sinPhi;
			D.v2 = A.v2;
			D.v3 = rho * cosTheta * sinPhi;
			vertexData[pi++] = D.v1;
			vertexData[pi++] = D.v2;
			vertexData[pi++] = D.v3;
			vertexData[pi++] = 1;
			vertexData[ci++] = surfaceColor.red;
			vertexData[ci++] = surfaceColor.green;
			vertexData[ci++] = surfaceColor.blue;
			vertexData[ci++] = surfaceColor.alpha;
			N = D.getNormal();
			vertexData[ni++] = N.v1;
			vertexData[ni++] = N.v2;
			vertexData[ni++] = N.v3;
			indexData[i++] = idx++;
		}
		phiUp = phiDown;
		phiDown = phiUp + stepY;
	}

	// The bottom pole
	//  A-----C
	//   \   /
	//    \ /
	//     B
	phi = phiUp;
	for ( theta = 0; theta < 360.0f; theta += stepX )
	{
		thetaR = DegToRad( theta );
		sinTheta = sinf( thetaR );
		cosTheta = cosf( thetaR );
		phiR = DegToRad( phi );
		sinPhi = sinf( phiR );
		cosPhi = cosf( phiR );
		A.v1 = rho * sinTheta * sinPhi;
		A.v2 = rho * cosPhi;
		A.v3 = rho * cosTheta * sinPhi;
		vertexData[pi++] = A.v1;
		vertexData[pi++] = A.v2;
		vertexData[pi++] = A.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = A.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;

		B.v1 = 0;
		B.v2 = radius;
		B.v3 = 0;
		vertexData[pi++] = B.v1;
		vertexData[pi++] = B.v2;
		vertexData[pi++] = B.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = B.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;

		thetaR = DegToRad( theta + stepX );
		sinTheta = sinf( thetaR );
		cosTheta = cos( thetaR );
		C.v1 = rho * sinTheta * sinPhi;
		C.v2 = A.v2;
		C.v3 = rho * cosTheta * sinPhi;
		vertexData[pi++] = C.v1;
		vertexData[pi++] = C.v2;
		vertexData[pi++] = C.v3;
		vertexData[pi++] = 1;
		vertexData[ci++] = surfaceColor.red;
		vertexData[ci++] = surfaceColor.green;
		vertexData[ci++] = surfaceColor.blue;
		vertexData[ci++] = surfaceColor.alpha;
		N = C.getNormal();
		vertexData[ni++] = N.v1;
		vertexData[ni++] = N.v2;
		vertexData[ni++] = N.v3;
		indexData[i++] = idx++;
	}

	ElementArray arr = { vertexData, indexData };
	return arr;
}
