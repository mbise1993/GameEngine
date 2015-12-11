#include "LightSource.h"

LightSource::LightSource() : position({ 0, 0, 0 }), lightIntensity(0.0f), currentIntensity(0.0f)
{
	this->position = { 0, 0, 0 };
	this->lightIntensity = 0.0f;
	this->currentIntensity = 0.0f;
	this->state = LIGHT_ON;
}

LightSource::~LightSource()
{
}

void LightSource::setIntensity(float lightIntensity)
{
	this->lightIntensity = lightIntensity;
	this->currentIntensity = lightIntensity;
}

void LightSource::setState(LightState state)
{
	this->state = state;
	if (this->state == LIGHT_ON)
		this->currentIntensity = this->lightIntensity;

	if (this->state == LIGHT_OFF)
		this->currentIntensity = 0.0f;
}