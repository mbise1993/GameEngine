#pragma once
#ifndef LOCAL_LIGHT_SOURCE
#define LOCAL_LIGHT_SOURCE

#include "BaseObject.h"
#include "MathUtil.h"

class LightSource : public BaseObject
{
protected:
	Vector3f position;
	float intensity;

public:
	LightSource();
	virtual ~LightSource();

	void setPosition( float x, float y, float z ) { this->position = { x, y, z }; }

	const Vector3f& getPosition() { return this->position; }

	void setIntensity( float intensity ) { this->intensity = intensity; }

	float getIntensity() { return this->intensity; }

	virtual void update( float elapsedSeconds ) {}
};

#endif

