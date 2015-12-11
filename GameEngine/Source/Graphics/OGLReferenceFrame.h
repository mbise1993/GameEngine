#pragma once
#ifndef OGL_REFERENCE_FRAME
#define OGL_REFERENCE_FRAME

#include <glm\glm.hpp>

class OGLReferenceFrame
{
public:
	glm::mat4 orientation;

public:
	OGLReferenceFrame();
	virtual ~OGLReferenceFrame();

	// Sets the position 
	void setPosition(const glm::vec3& position);
	void setPosition(float x, float y, float z);

	// Gets the position
	glm::vec3 getPosition() { return glm::vec3(orientation[3]); }

	void rotateZ(float degrees);
	void rotateY(float degrees);
	void rotateX(float degrees);

	void move(const glm::vec3& direction, float speed);
	void moveRight(float speed);
	void moveLeft(float speed);
	void moveForward(float speed);
	void moveBackward(float speed);
	void translate(float x, float y, float z);
};

#endif

