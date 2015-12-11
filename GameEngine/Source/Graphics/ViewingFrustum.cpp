#include "ViewingFrustum.h"

ViewingFrustum::ViewingFrustum()
{
	set(60.0f, 1.78f, 0.1f, 100.0f);
}

ViewingFrustum::~ViewingFrustum()
{
}

void ViewingFrustum::set(float fieldOfView, float aspectRatio, float zNear, float zFar)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->zNear = zNear;
	this->zFar = zFar;
}
