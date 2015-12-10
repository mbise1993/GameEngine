#include "Cuboid.h"
#include "ObjectGenerator.h"
#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"

Cuboid::Cuboid( const std::string& name, float width, float depth, float height, RGBA faceColor ) :
	BaseCuboid( name, width, depth, height, faceColor )
{
	this->generate();
}

Cuboid::~Cuboid()
{}

void Cuboid::generate()
{
	//this->setIndexedArrayType();
	ElementArray arr = ObjectGenerator::generateCuboid(
		this->width, this->depth, this->height, this->faceColor );
	//this->setVertexData(arr.vertexData);
	//this->setIndexData(arr.indexData);
	//VBOObject* vboObject = OGLObject::createElementArray(
	//	"triangles", arr.vertexData, arr.indexData, GL_TRIANGLES);
	//this->sendBuffersToGPU(vboObject);
	this->createElementArrayPCN( "Triangles", arr.vertexData, arr.indexData, GL_TRIANGLES );
}
