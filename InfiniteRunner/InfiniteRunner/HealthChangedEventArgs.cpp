#include "HealthChangedEventArgs.h"

HealthChangedEventArgs::HealthChangedEventArgs( int healthDelta ) :
	EventArgs( HEALTH_CHANGED_EVENT )
{
	this->healthDelta = healthDelta;
}

HealthChangedEventArgs::~HealthChangedEventArgs()
{

}