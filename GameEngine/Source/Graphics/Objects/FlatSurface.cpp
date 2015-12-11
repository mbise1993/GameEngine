#include "FlatSurface.h"

FlatSurface::FlatSurface(const std::string& name, float width, float depth, RGBA color) :
	OGL3DObject(name)
{
	this->width = width;
	this->depth = depth;
	this->color = color;

	generate();
}

FlatSurface::~FlatSurface()
{
	
}

void FlatSurface::generate()
{
	float halfWidth = this->width / 2.0f;
	float halfDepth = this->depth / 2.0f;

	// First lower left
	Vertex firstLowerLeftVertex;
	firstLowerLeftVertex.positionX = -halfWidth;
	firstLowerLeftVertex.positionY = 0.0f;
	firstLowerLeftVertex.positionZ = halfDepth;
	firstLowerLeftVertex.positionW = 1.0f;
	firstLowerLeftVertex.normalX = 0.0f;
	firstLowerLeftVertex.normalY = 1.0f;
	firstLowerLeftVertex.normalZ = 0.0f;
	firstLowerLeftVertex.red = color.red;
	firstLowerLeftVertex.green = color.green;
	firstLowerLeftVertex.blue = color.blue;
	firstLowerLeftVertex.alpha = color.alpha;
	// First lower right
	Vertex firstLowerRightVertex;
	firstLowerRightVertex.positionX = halfWidth;
	firstLowerRightVertex.positionY = 0.0f;
	firstLowerRightVertex.positionZ = halfDepth;
	firstLowerRightVertex.positionW = 1.0f;
	firstLowerRightVertex.normalX = 0.0f;
	firstLowerRightVertex.normalY = 1.0f;
	firstLowerRightVertex.normalZ = 0.0f;
	firstLowerRightVertex.red = color.red;
	firstLowerRightVertex.green = color.green;
	firstLowerRightVertex.blue = color.blue;
	firstLowerRightVertex.alpha = color.alpha;
	// First upper right
	Vertex firstUpperRightVertex;
	firstUpperRightVertex.positionX = halfWidth;
	firstUpperRightVertex.positionY = 0.0f;
	firstUpperRightVertex.positionZ = -halfDepth;
	firstUpperRightVertex.positionW = 1.0f;
	firstUpperRightVertex.normalX = 0.0f;
	firstUpperRightVertex.normalY = 1.0f;
	firstUpperRightVertex.normalZ = 0.0f;
	firstUpperRightVertex.red = color.red;
	firstUpperRightVertex.green = color.green;
	firstUpperRightVertex.blue = color.blue;
	firstUpperRightVertex.alpha = color.alpha;
	// Second lower left
	Vertex secondLowerLeftVertex;
	secondLowerLeftVertex.positionX = -halfWidth;
	secondLowerLeftVertex.positionY = 0.0f;
	secondLowerLeftVertex.positionZ = halfDepth;
	secondLowerLeftVertex.positionW = 1.0f;
	secondLowerLeftVertex.normalX = 0.0f;
	secondLowerLeftVertex.normalY = 1.0f;
	secondLowerLeftVertex.normalZ = 0.0f;
	secondLowerLeftVertex.red = color.red;
	secondLowerLeftVertex.green = color.green;
	secondLowerLeftVertex.blue = color.blue;
	secondLowerLeftVertex.alpha = color.alpha;
	// Second upper right
	Vertex secondUpperRightVertex;
	secondUpperRightVertex.positionX = halfWidth;
	secondUpperRightVertex.positionY = 0.0f;
	secondUpperRightVertex.positionZ = -halfDepth;
	secondUpperRightVertex.positionW = 1.0f;
	secondUpperRightVertex.normalX = 0.0f;
	secondUpperRightVertex.normalY = 1.0f;
	secondUpperRightVertex.normalZ = 0.0f;
	secondUpperRightVertex.red = color.red;
	secondUpperRightVertex.green = color.green;
	secondUpperRightVertex.blue = color.blue;
	secondUpperRightVertex.alpha = color.alpha;
	// Second upper left
	Vertex secondUpperLeftVertex;
	secondUpperLeftVertex.positionX = -halfWidth;
	secondUpperLeftVertex.positionY = 0.0f;
	secondUpperLeftVertex.positionZ = -halfDepth;
	secondUpperLeftVertex.positionW = 1.0f;
	secondUpperLeftVertex.normalX = 0.0f;
	secondUpperLeftVertex.normalY = 1.0f;
	secondUpperLeftVertex.normalZ = 0.0f;
	secondUpperLeftVertex.red = color.red;
	secondUpperLeftVertex.green = color.green;
	secondUpperLeftVertex.blue = color.blue;
	secondUpperLeftVertex.alpha = color.alpha;

	// Vertex data
	std::vector<Vertex>* vertexData = new std::vector<Vertex>
	{
		firstLowerLeftVertex, firstLowerRightVertex, firstUpperRightVertex,
		secondLowerLeftVertex, secondUpperRightVertex, secondUpperLeftVertex
	};

	this->setArrayBufferType();
	VBOObject* vboObject = OGLObject::createVBOObject("triangles", vertexData, GL_TRIANGLES);
	this->addVBOObject(vboObject);
}