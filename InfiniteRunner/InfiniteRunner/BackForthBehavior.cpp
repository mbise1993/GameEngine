#include "BackForthBehavior.h"
#include "OGLObject.h"

BackForthBehavior::BackForthBehavior( float maxDistance )
{
	this->state = MOVING_FORWARD;
	this->maxDistance = maxDistance;
	this->distanceMoved = 0;
}

BackForthBehavior::~BackForthBehavior()
{}

void BackForthBehavior::update( GameObject *object, float elapsedSeconds )
{
	OGLObject* obj = (OGLObject*)object;
	float delta = obj->getSpeed() * elapsedSeconds;
	this->distanceMoved += delta;
	switch ( this->state )
	{
	case MOVING_BACKWARD:
		if ( this->distanceMoved >= this->maxDistance )
		{
			this->state = MOVING_FORWARD;
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		obj->referenceFrame.moveBackward( delta );
		break;
	case MOVING_FORWARD:
		if ( this->distanceMoved >= this->maxDistance )
		{
			this->state = MOVING_BACKWARD;
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		obj->referenceFrame.moveForward( delta );
		break;
	}
}
