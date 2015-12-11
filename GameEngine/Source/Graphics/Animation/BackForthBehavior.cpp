#include "BackForthBehavior.h"

BackForthBehavior::BackForthBehavior(float maxDistance, glm::vec3 dir)
{
	this->state = MOVING_FORWARD;
	this->maxDistance = maxDistance;
	this->dir = dir;
	this->distanceMoved = 0;
}

BackForthBehavior::~BackForthBehavior()
{
}

void BackForthBehavior::update(GameObject *object, float elapsedSeconds)
{
	OGLObject* obj = (OGLObject*)object;
	float delta = 10.0f * elapsedSeconds;
	this->distanceMoved += delta;
	switch (this->state) {
	case MOVING_BACKWARD:
		if (this->distanceMoved >= this->maxDistance) {
			this->state = MOVING_FORWARD;
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		obj->referenceFrame.move(-this->dir, delta);
		break;
	case MOVING_FORWARD:
		if (this->distanceMoved >= this->maxDistance) {
			this->state = MOVING_BACKWARD;
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		obj->referenceFrame.move(this->dir, delta);
		break;
	}
}
