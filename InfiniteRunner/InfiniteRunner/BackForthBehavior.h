#pragma once
#ifndef BACK_FORTH_BEHAVIOR
#define BACK_FORTH_BEHAVIOR

#include "IBehavior.h"

class BackForthBehavior :
	public IBehavior
{
public:
	enum State { MOVING_FORWARD, MOVING_BACKWARD };

protected:
	State state;
	float maxDistance;
	float distanceMoved;

public:
	BackForthBehavior( float maxDistance = 10.0f );
	virtual ~BackForthBehavior();

	void update( GameObject *object, float elapsedSeconds );
};

#endif

