#include "DestroyerMissile.h"
#include "PlayerController.h"
#include "OGL3DObject.h"

DestroyerMissile::DestroyerMissile( PlayerController * player )
{
	this->player = player;
	this->isActive = false;
	this->justActivated = false;
	this->frame.scaleBy( 0.5f );
}

DestroyerMissile::~DestroyerMissile()
{}

void DestroyerMissile::update( GameObject * object, float elapsedSeconds )
{
	OGL3DObject* amo = (OGL3DObject*)object;

	if ( this->justActivated )
	{
		this->justActivated = false;
		this->isActive = true;
		this->frame.setPosition( this->startingPosition - this->player->getPosition() );

	}

	amo->setVisibility( this->isActive );

	if ( this->isActive )
	{
		// Rotate
		this->frame.rotateX( (float)(rand() % 180) );
		this->frame.rotateY( (float)(rand() % 180) );
		this->frame.rotateZ( (float)(rand() % 180) );

		// Get frame position
		glm::vec3 framePos = this->frame.getPosition();

		// Move closer
		framePos.x = this->closeDistance( framePos.x, 1.0f );
		framePos.y = this->closeDistance( framePos.y, 1.0f );
		framePos.z = this->closeDistance( framePos.z, 1.0f );

		// Set frame position
		this->frame.setPosition( framePos );

		// Set amo position
		amo->referenceFrame.orientation = this->player->getPlayerObject()->referenceFrame.orientation * this->frame.orientation;

		if ( framePos.x + framePos.y + framePos.z <= 1.0f )
		{
			this->player->addDamage( 1.0f );
			this->setInactive();
		}
	}
}

void DestroyerMissile::setActive( glm::vec3 position )
{
	this->startingPosition = position;
	this->justActivated = true;
}

void DestroyerMissile::setInactive()
{
	this->isActive = false;
}

float DestroyerMissile::closeDistance( float value, float speed )
{
	if ( value > 0 )
	{
		value -= speed;
		if ( value <= 0.0f )
		{
			value = 0.0f;
		}
	}
	else if ( value < 0 )
	{
		value += speed;
		if ( value >= 0.0f )
		{
			value = 0.0f;
		}
	}

	return value;
}
