#include "Controller.h"

Controller::Controller( OGL3DObject* target, float maximumHealth )
{
	this->target = target;
	assert( this->target != nullptr );

	this->target->setBehavior( this );

	this->timeAlive = 0.0f;
	this->maxHealth = maximumHealth;
	this->health = maximumHealth;
}

Controller::~Controller()
{}

float Controller::getHealthPercentage()
{
	if ( this->maxHealth == 0 ) return 0.0f;
	return (this->health / this->maxHealth);
}

void Controller::setHealth( float amount )
{
	if ( amount < 0 )
	{
		amount = 0;
	}
	else if ( amount > this->maxHealth )
	{
		amount = this->maxHealth;
	}

	this->health = amount;
}

void Controller::setMaxHealth( float amount )
{
	if ( amount < 0 ) amount = 0;

	this->maxHealth = amount;

	if ( amount < this->health )
	{
		this->setHealth( amount );
	}
}

void Controller::update( GameObject * object, float elapsedSeconds )
{
	// Add to time alive
	this->timeAlive += elapsedSeconds;
}
