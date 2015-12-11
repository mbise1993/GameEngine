#pragma once
#ifndef CAMERA
#define CAMERA

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void setTarget(float x, float y, float z) = 0;

	virtual void update(float elapsedSeconds) = 0;
};

#endif

