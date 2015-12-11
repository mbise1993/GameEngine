#pragma once
#ifndef ROTATE_Y_BEHAVIOR
#define ROTATE_Y_BEHAVIOR

#include "IBehavior.h"

class RotateYBehavior :
	public IBehavior
{
protected:
	float speed;

public:
	RotateYBehavior(float speed);
	virtual ~RotateYBehavior();

	void update(GameObject *object, float elapsedSeconds);
};

#endif

