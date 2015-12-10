#pragma once
#ifndef OGL_GAME_OBJECT
#define OGL_GAME_OBJECT

#include "GameObject.h"

#include <string>

#include <gl\glew.h>

class LineBox;

// OGLGameObject -> GameObject -> BaseObject
class OGLGameObject : public GameObject
{
public:
	GLuint vao;

	// Used to visualize the bounding box
	LineBox* lineBox;

protected:
	GLuint shaderProgram;

public:
	OGLGameObject( const std::string& name );
	virtual ~OGLGameObject();

	virtual void setShaderProgram( GLuint shaderProgram )
	{
		this->shaderProgram = shaderProgram;
	}
};

#endif

