#pragma once
#ifndef BACK_FORTH_BEHAVIOR
#define BACK_FORTH_BEHAVIOR

#include "IBehavior.h"
#include "Objects/OGLObject.h"

class BackForthBehavior :
	public IBehavior
{
public:
	enum State { MOVING_FORWARD, MOVING_BACKWARD };

protected:
	State state;
	float maxDistance;
	glm::vec3 dir;
	float distanceMoved;

public:
	BackForthBehavior(float maxDistance, glm::vec3 dir);
	virtual ~BackForthBehavior();

	void update(GameObject *object, float elapsedSeconds);
};

#endif

