#include "LineBox.h"
#include "ObjectGenerator.h"
#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"

using std::string;

LineBox::LineBox( const string& name, float width, float depth, float height, RGBA lineColor ) :
	BaseCuboid( name, width, depth, height, lineColor )
{
	this->generate();
}

LineBox::~LineBox()
{}

void LineBox::generate()
{
	//this->setIndexedArrayType();
	ElementArray arr = ObjectGenerator::generateLineBoxIndexedArray(
		this->width, this->depth, this->height, this->faceColor );
	//this->setVertexData(arr.vertexData);
	//this->setIndexData(arr.indexData);
	//this->setNormalCount(0);
	//VBOObject* vboObject = OGLObject::createElementArray(
	//	"Lines", arr.vertexData, arr.indexData, GL_LINES);
	//this->sendBuffersToGPU(vboObject);
	this->createElementArrayPC( "Lines", arr.vertexData, arr.indexData, GL_LINES );
}

