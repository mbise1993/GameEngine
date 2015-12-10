#pragma once
#ifndef OGL_FIXED_TARGET_CAMERA
#define OGL_FIXED_TARGET_CAMERA

#include "OGLCamera.h"

class OGLObject;

class OGLFixedTargetCamera : public OGLCamera
{
protected:
	OGLObject* targetObject;
	float targetYDistance;
	float rho, phiDegrees, thetaDegrees;
	struct SphericalCoordinate
	{
		float rho, phi, theta;
	}cameraSpherical;

public:
	OGLFixedTargetCamera();
	virtual ~OGLFixedTargetCamera();

	void setTargetObject( OGLObject* object ) { this->targetObject = object; }

	OGLObject* getTargetObject() { return this->targetObject; }

	void setPosition( float distanceFromTarget, float targetYDistance, float upDownDegrees, float leftRightDegrees );

	void update( float elapsedSeconds );

protected:
	void updateOrientation();
};

#endif