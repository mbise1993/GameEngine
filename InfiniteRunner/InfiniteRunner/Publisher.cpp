#include "Publisher.h"

Publisher::Publisher()
{}

Publisher::~Publisher()
{}

void Publisher::registerSubscriber( EventTypes eventType, Subscriber* subscriber )
{
	this->subscribers[eventType].push_back( subscriber );
}

void Publisher::publishEvent( EventArgs* eventArgs )
{
	EventTypes eventType = eventArgs->getEventType();
	if ( this->subscribers.count( eventType ) != 0 )
	{
		for ( auto subscriber : this->subscribers[eventType] )
		{
			subscriber->eventHandler( eventArgs );
		}
	}
}