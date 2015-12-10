#pragma once
#ifndef OGL_OBJECT
#define OGL_OBJECT

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <map>
#include <string>

#include "OGLGameObject.h"
#include "OGLReferenceFrame.h"
#include "OGLArrayBuffer.h"
#include "EngineTypes.h"


struct Component
{
	unsigned int count;
	GLenum type;
	unsigned int bytesToNext;
	unsigned int bytesToFirst;
};

struct VBOObject
{
	std::string name;
	GLuint vbo;
	GLuint ibo;
	GLenum primitiveType;
	void * buffer;
	unsigned int bufferSizeInBytes;
	unsigned int numberOfVertices;
	void * indexData;
	unsigned int indexSizeInBytes;
	unsigned int numberOfIndexes;
	Component positionComponent;
	Component colorComponent;
	Component normalComponent;
	Component textureComponent;
};

enum VBOType { ARRAY_BUFFER, INDEXED_ARRAY };

class OGLBufferedObject;
class OGLElementArrayBuffer;

// OGLObject -> OGLGameObject -> GameObject -> BaseObject
class OGLObject : public OGLGameObject
{
public:
	OGLReferenceFrame referenceFrame;

protected:
	std::map<std::string, OGLBufferedObject*> buffers;

	float speed;

	GLenum targetBufferObject;

	//private:
	//	unsigned int positionCount, colorCount, normalCount, textureCount;

public:
	OGLObject( std::string name );

	virtual ~OGLObject();

	void update( float elapsedSeconds );

	void render() = 0;

	virtual void render( OGLReferenceFrame& frame ) = 0;

	void setTexture( Texture* texture );

	void setShaderProgram( GLuint shaderProgram );

	void sendBuffersToGPU();

	OGLBufferedObject* getBufferedObject( const std::string& name );

	OGLArrayBuffer* createArrayBuffer( std::string name, float* vertexData, GLenum primitiveType );

	OGLElementArrayBuffer* createElementArray( const std::string& name, float* vertexData, index_buffer_t* indexData, GLenum primitiveType = GL_TRIANGLES );

	void createElementArrayPC( const std::string& name, float* vertexData, index_buffer_t* indexData, GLenum primitiveType = GL_TRIANGLES );
	void createElementArrayPCN( const std::string& name, float* vertexData, index_buffer_t* indexData, GLenum primitiveType = GL_TRIANGLES );
	void createElementArrayPCNT( const std::string& name, float* vertexData, index_buffer_t* indexData, GLenum primitiveType = GL_TRIANGLES );

	void setSpeed( float speed )
	{
		this->speed = speed;
	}

	float getSpeed()
	{
		return this->speed;
	}

	void sendMaterialDataToGPU();


protected:
	void renderBufferedObjects();

};

#endif

