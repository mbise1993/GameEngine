#include "RotateYBehavior.h"
#include "Objects/OGL3DObject.h"

RotateYBehavior::RotateYBehavior(float speed)
{
	this->speed = speed;
}

RotateYBehavior::~RotateYBehavior()
{
}

void RotateYBehavior::update(GameObject *object, float elapsedSeconds)
{
	OGL3DObject *theObject = (OGL3DObject*)object;
	theObject->referenceFrame.rotateY(this->speed * elapsedSeconds);
}
