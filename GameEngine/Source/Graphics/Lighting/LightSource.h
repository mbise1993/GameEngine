#pragma once
#ifndef LOCAL_LIGHT_SOURCE
#define LOCAL_LIGHT_SOURCE

#include "MathUtil.h"

enum LightState { LIGHT_ON, LIGHT_OFF };

class LightSource
{
protected:
	LightState state;
	Vector3f position;
	float lightIntensity;
	float currentIntensity;

public:
	LightSource();
	virtual ~LightSource();

	void setPosition(float x, float y, float z) { this->position = { x, y, z }; }
	
	const Vector3f& getPosition() { return this->position; }

	void setIntensity(float lightIntensity);
	float getIntensity() { return this->currentIntensity; }

	void setState(LightState state);
	LightState getState() { return this->state; }

	virtual void update(float elapsedSeconds) {}
};

#endif

