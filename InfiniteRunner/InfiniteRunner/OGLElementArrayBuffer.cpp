#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"
#include "Texture.h"

using std::string;

OGLElementArrayBuffer::OGLElementArrayBuffer()
{
	this->type = GL_ELEMENT_ARRAY_BUFFER;
	this->arrayBuffer = new OGLArrayBuffer();
}

OGLElementArrayBuffer::~OGLElementArrayBuffer()
{
	delete this->arrayBuffer;
}

void OGLElementArrayBuffer::render()
{
	this->arrayBuffer->select();
	this->drawObject();
	this->arrayBuffer->deselect();
}

void OGLElementArrayBuffer::drawObject()
{
	glBindBuffer( this->type, this->id );
	glDrawElements(
		this->arrayBuffer->getPrimitiveType(),
		this->numberOfElements, OGL_INDEX_T, 0 );
}

void OGLElementArrayBuffer::set( const string& name, index_buffer_t* buffer )
{
	this->name = name;
	this->buffer = &buffer[1];
	this->baseBuffer = buffer;
	this->numberOfElements = (GLuint)buffer[0];
	this->sizeInBytes = this->numberOfElements * sizeof( index_buffer_t );
}

void OGLElementArrayBuffer::setTexture( Texture* texture )
{
	this->texture = texture;
	this->arrayBuffer->setTexture( texture );
}

void OGLElementArrayBuffer::setShaderHandle( GLuint shaderHandle )
{
	this->shaderHandle = shaderHandle;
	this->arrayBuffer->setShaderHandle( shaderHandle );
}

void OGLElementArrayBuffer::generateBuffer()
{
	this->arrayBuffer->generateBuffer();
	OGLBufferedObject::generateBuffer();
}

void OGLElementArrayBuffer::sendBufferToGPU()
{
	OGLBufferedObject::sendBufferToGPU();
	this->arrayBuffer->sendBufferToGPU();
}
