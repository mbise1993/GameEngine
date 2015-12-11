#pragma once
#ifndef CAT
#define CAT

#include "OGL3DCompositeObject.h"

class OGLObject;

class Cat : public OGL3DCompositeObject
{
protected:
	OGLObject* body;
	OGLObject* head;
	OGLObject* ears;
	OGLObject* tail;
	OGLObject* frontLeftLeg;
	OGLObject* frontRightLeg;
	OGLObject* backLeftLeg;
	OGLObject* backRightLeg;

	float legsSpeed;
	float leftLegsAngleX, rightLegsAngleX;
	float leftLegsSpeed, rightLegsSpeed;

public:
	Cat(const std::string& name, float legsSpeed, OGLObject* body, OGLObject* head, OGLObject* ears, OGLObject* tail, 
		OGLObject* frontLeftLeg, OGLObject* frontRightLeg, OGLObject* backLeftLeg, OGLObject* backRightLeg);
	~Cat();

	void setShaderProgram(GLuint shaderProgram);

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render();
};

#endif