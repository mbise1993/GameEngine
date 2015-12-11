#include "PatrolBehavior.h"
#include "Objects/OGLObject.h"

PatrolBehavior::PatrolBehavior(float maxDistance, float moveSpeed, float maxTurn, float turnSpeed)
{
	this->state = MOVING;
	this->maxDistance = maxDistance;
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;
	this->maxTurn = maxTurn;
	this->distanceMoved = 0;
	this->distanceTurned = 0;
}

PatrolBehavior::~PatrolBehavior()
{
}

void PatrolBehavior::update(GameObject *object, float elapsedSeconds)
{
	OGLObject* obj = (OGLObject*)object;
	float delta;

	switch (this->state)
	{
		case MOVING:
			delta = this->moveSpeed * elapsedSeconds;
			this->distanceMoved += delta;

			if (this->distanceMoved >= this->maxDistance)
			{
				this->state = TURNING;
				delta = this->distanceMoved - this->maxDistance;
				this->distanceMoved = 0;
			}

			obj->referenceFrame.moveForward(delta);
			break;
		case TURNING:
			delta = this->turnSpeed * elapsedSeconds;
			this->distanceTurned += delta;

			if (this->distanceTurned >= this->maxTurn)
			{
				this->state = MOVING;
				delta -= (this->maxTurn - this->distanceTurned);
				this->distanceTurned = 0;
			}

			obj->referenceFrame.rotateY(delta);
			break;
	}
}
