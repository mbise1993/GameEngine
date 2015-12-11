#pragma once
#ifndef GUITARIST
#define GUITARIST

#include "OGL3DCompositeObject.h"
#include "Cuboid.h"

class OGLObject;

class Guitarist : public OGL3DCompositeObject
{
protected:
	OGLObject* torso;
	OGLObject* legs;
	OGLObject* head;
	OGLObject* leftArm;
	OGLObject* rightBicep;
	OGLObject* rightForearm;
	Cuboid* rightForearmCuboid;

	float strumSpeed, currentStrumSpeed;
	float strumAngleZ;

public:
	Guitarist(const std::string& name, float strumSpeed, OGLObject* torso, OGLObject* legs, OGLObject* head,
		OGLObject* leftArm, OGLObject* rightBicep, OGLObject* rightForearm);
	~Guitarist();

	void setShaderProgram(GLuint shaderProgram);

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render();
};

#endif