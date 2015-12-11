#include "OGLCamera.h"

OGLCamera::OGLCamera()
{
}

OGLCamera::~OGLCamera()
{
}

void OGLCamera::setTarget(float x, float y, float z)
{
	this->target = glm::vec3(x, y, z);
	updateOrientation();
}

void OGLCamera::setUp(float x, float y, float z)
{
	this->up = glm::vec3(x, y, z);
	updateOrientation();
}

void OGLCamera::updateOrientation()
{
}
