#pragma once
#ifndef OGL_ELEMENT_ARRAY_BUFFER
#define OGL_ELEMENT_ARRAY_BUFFER

#include "OGLBufferedObject.h"
#include "EngineTypes.h"

#include <string>

class OGLArrayBuffer;

class OGLElementArrayBuffer : public OGLBufferedObject
{
protected:
	OGLArrayBuffer* arrayBuffer;

public:
	OGLElementArrayBuffer();

	virtual ~OGLElementArrayBuffer();

	OGLArrayBuffer* getArrayBuffer() { return this->arrayBuffer; }

	void setArrayBuffer( OGLArrayBuffer* buffer ) { this->arrayBuffer = buffer; }

	void setTexture( Texture* texture );

	void set( const std::string& name, index_buffer_t* buffer );

	void setShaderHandle( GLuint shaderHandle );

	void render();

	void drawObject();

	void generateBuffer();

	void sendBufferToGPU();
};

#endif

