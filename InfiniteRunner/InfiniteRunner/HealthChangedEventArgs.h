#pragma once
#ifndef HEALTH_CHANGED_EVENT_ARGS
#define HEALTH_CHANGED_EVENT_ARGS

#include "EventArgs.h"

class HealthChangedEventArgs : public EventArgs
{
private:
	int healthDelta;

public:
	HealthChangedEventArgs( int healthDelta );
	virtual ~HealthChangedEventArgs();

	int getHealthDelta() { return this->healthDelta; }
};

#endif
