#pragma once
#ifndef EVENT_ARGS
#define EVENT_ARGS

#include "EventTypes.h"

class EventArgs
{
protected:
	EventTypes eventType;

public:
	EventArgs( EventTypes eventType ) : eventType( eventType ) {}
	virtual ~EventArgs() {}

	EventTypes getEventType() { return this->eventType; }
};

#endif