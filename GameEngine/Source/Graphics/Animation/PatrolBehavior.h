#pragma once
#ifndef PATROL_BEHAVIOR
#define PATROL_BEHAVIOR

#include "IBehavior.h"

class GameObject;

class PatrolBehavior : public IBehavior
{
	public:
		enum State { MOVING, TURNING };

	protected:
		State state;
		float maxDistance;
		float distanceMoved;
		float distanceTurned;
		float moveSpeed;
		float turnSpeed;
		float maxTurn;

	public:
		PatrolBehavior(float maxDistance, float moveSpeed, float maxTurn, float turnSpeed);
		virtual ~PatrolBehavior();

		void update(GameObject *object, float elapsedSeconds);
};

#endif