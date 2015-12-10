#pragma once
#ifndef OGL_ARRAY_BUFFER
#define OGL_ARRAY_BUFFER

#include "OGLBufferedObject.h"

#include <unordered_map>
#include <string>

class OGLArrayBuffer : public OGLBufferedObject
{
public:
	struct Component
	{
		GLuint layout;
		GLuint count;
		GLenum type;
		GLuint bytesToNext;
		GLuint bytesToFirst;
		unsigned int startIdx;
	};

protected:
	GLenum primitiveType;

	std::unordered_map<std::string, Component*> components;

	GLuint numberOfVertices;

public:
	OGLArrayBuffer();

	virtual ~OGLArrayBuffer();

	/*
	Instructs OGL to enable the components
	*/
	void enableComponents();

	/*
	Instructs OGL to disable the components
	*/
	void disableComponents();

	/*
	Gets a component by name.
	*/
	Component* getComponent( const std::string& name );

	/*
	Adds a component with the specified name
	*/
	virtual OGLArrayBuffer::Component* addComponent( const std::string& name, GLuint count = 4 );

	/*
	Sets up the specified component
	*/
	virtual void setupComponent(
		const std::string& name,
		GLuint layout = 0, GLenum type = GL_FLOAT,
		GLuint bytesToNext = 0, GLuint bytesToFirst = 0, GLuint startIdx = 0 );

	/*
	Renders the buffer
	*/
	virtual void render();

	/*
	Selects the buffer and shader
	*/
	void select();

	/*
	De-selects the buffer and shader
	*/
	void deselect();

	/*
	Returns the number of vertices in the buffer
	*/
	GLuint getNumberOfVertices();

	/*
	Returns the primative type of the buffer
	*/
	GLenum getPrimitiveType() const { return this->primitiveType; }

	/*
	Sets the buffer data
	*/
	virtual void set( const std::string& name, float* buffer, GLenum primitiveType );

protected:
	/*
	Draws the object
	*/
	virtual void drawObject();

	void updateVerticeCount();
};

#endif

