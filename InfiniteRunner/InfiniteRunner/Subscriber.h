#pragma once
#ifndef SUBSCRIBER
#define SUBSCRIBER

#include "EventArgs.h"

class Subscriber
{
public:
	virtual void eventHandler( EventArgs* eventArgs ) = 0;
};

#endif