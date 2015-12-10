#pragma once
#ifndef PUBLISHER
#define PUBLISHER

#include "Subscriber.h"
#include <map>
#include <vector>

class Publisher
{
private:
	std::map<EventTypes, std::vector<Subscriber*>> subscribers;

public:
	Publisher();
	virtual ~Publisher();

	void registerSubscriber( EventTypes eventType, Subscriber* subscriber );

	void publishEvent( EventArgs* eventArgs );
};

#endif