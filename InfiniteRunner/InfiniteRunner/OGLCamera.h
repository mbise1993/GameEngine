#pragma once
#ifndef OGL_CAMERA
#define OGL_CAMERA

#include "Camera.h"

#include <glm\glm.hpp>

class OGLCamera :
	public Camera
{
protected:
	glm::vec3 up;
	glm::vec3 target;

public:
	glm::mat4 orientation;

public:
	OGLCamera();
	virtual ~OGLCamera();

	glm::vec3 getPosition() { return glm::vec3( this->orientation[3] ); }

	virtual void setTarget( float x, float y, float z );

	virtual void setUp( float x, float y, float z );

protected:
	virtual void updateOrientation();
};

#endif

