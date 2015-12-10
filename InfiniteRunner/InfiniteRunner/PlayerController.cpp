#include "PlayerController.h"
#include "PCInputSystem.h"
#include "ScoreChangedEventArgs.h"
#include "HealthChangedEventArgs.h"
#include "ChunkSystem.h"
#include "GameOverEventArgs.h"
#include "GameResetEventArgs.h"
#include <Windows.h>

PlayerController::PlayerController( OGL3DObject* playerObject,
	float maximumHealth, float strafeSpeed,
	float acclerationPerSecond, float startingSpeed )
	: Controller( playerObject, maximumHealth )
{
	this->strafeSpeed = strafeSpeed;
	this->speed = this->startingSpeed = startingSpeed;
	this->accelerationPerSecond = acclerationPerSecond;
	this->startAcclerationPerSecond = accelerationPerSecond;
	this->strafeDirection = StrafeDirections::None;
	this->startingPosition = glm::vec3( 0 );
	this->rollAngle = 0.0f;
	this->rollMax = 10.0f;
	this->rollSpeed = 50.0f;
	this->gameOver = false;
}

PlayerController::~PlayerController()
{}

void PlayerController::setHealth( float amount )
{
	// Call base
	Controller::setHealth( amount );

	// Update health HUD
	HealthChangedEventArgs eventArgs( (int)(this->getHealthPercentage() * 100.0f) );
	this->target->publishEvent( &eventArgs );

	// If health is 0 issue game over
	if ( amount <= 0 )
	{
		GameOverEventArgs args( this->getScore() );
		this->target->publishEvent( &args );
	}
}

void PlayerController::processInputs( PCInputSystem * inputSystem )
{
	//Check if gameover currently active(bit of a hack but works good enough)
	if ( !this->gameOver )
	{
		// Strafeing
		if ( (inputSystem->keys[VK_LEFT]) || (inputSystem->keys['A']) )
		{
			this->strafeDirection = StrafeDirections::Left;
		}
		else if ( (inputSystem->keys[VK_RIGHT]) || (inputSystem->keys['D']) )
		{
			this->strafeDirection = StrafeDirections::Right;
		}
	}
	else if ( this->gameOver && inputSystem->keys['R'] )
	{
		GameResetEventArgs eventArgs = GameResetEventArgs();
		this->target->publishEvent( &eventArgs );
	}

}

void PlayerController::update( GameObject * object, float elapsedSeconds )
{
	//Don't update score if game over active
	if ( !this->gameOver )
	{
		// Call base
		Controller::update( object, elapsedSeconds );

		// Update speed
		this->speed += (this->accelerationPerSecond * elapsedSeconds);

		// Move forward
		this->moveForward( this->speed * elapsedSeconds );

		// Update HUD score
		ScoreChangedEventArgs eventArgs( this->getScore() );
		this->target->publishEvent( &eventArgs );
	}

	// Strafe movement, and roll animation
	this->performStrafeAndRoll( elapsedSeconds );
}

void PlayerController::checkForCollisions( ChunkSystem * chunkSystem )
{
	if ( chunkSystem->checkCollision( this->target ) )
	{
		this->addDamage( 5 );
	}
}

void PlayerController::setStartingPosition( glm::vec3 position )
{
	this->startingPosition = position;
	this->target->referenceFrame.setPosition( position );
}

void PlayerController::resetPlayer()
{
	// Reset speed
	this->speed = this->startingSpeed;

	// Reset position TODO: Why does this screw up the collision system?
	//this->target->referenceFrame.setPosition( this->startingPosition );

	// Reset time alive
	this->timeAlive = 0;

	// Reset health
	this->setHealth( this->maxHealth );

	//reset game over flag
	this->gameOver = false;

	// Reset acceleration
	this->accelerationPerSecond = startAcclerationPerSecond;
}

void PlayerController::pausePlayer()
{
	this->speed = 0.0f;
	this->accelerationPerSecond = 0.0f;
	this->gameOver = true;
}

void PlayerController::eventHandler( EventArgs * eventArgs )
{
	switch ( eventArgs->getEventType() )
	{
	case GAME_OVER_EVENT:
		this->pausePlayer();
		break;
	case GAME_RESET_EVENT:
		this->resetPlayer();
		break;
	}
}

void PlayerController::performStrafeAndRoll( float elapsedSeconds )
{
	float strafeDistance = 0;
	float rollDelta = 0;
	float newAngle = this->rollAngle;
	float localRollMax = 0;

	// Strafe?
	switch ( this->strafeDirection )
	{
	case StrafeDirections::Left:
		strafeDistance = (this->strafeSpeed + (this->speed * 0.1f)) * elapsedSeconds;
		this->target->referenceFrame.translateWorld( strafeDistance, 0, 0 );

		rollDelta = -(this->rollSpeed * elapsedSeconds);
		localRollMax = -this->rollMax;
		break;

	case StrafeDirections::Right:
		strafeDistance = -(this->strafeSpeed + (this->speed * 0.1f)) * elapsedSeconds;
		this->target->referenceFrame.translateWorld( strafeDistance, 0, 0 );

		rollDelta = (this->rollSpeed * elapsedSeconds);
		localRollMax = this->rollMax;
		break;

	default:
		if ( this->rollAngle > 0 )
		{
			rollDelta = -(this->rollSpeed * elapsedSeconds);
			localRollMax = 0;
		}
		else if ( this->rollAngle < 0 )
		{
			rollDelta = (this->rollSpeed * elapsedSeconds);
			localRollMax = 0;
		}
	}
	this->strafeDirection = StrafeDirections::None;

	// Roll bounds check angle
	newAngle += rollDelta;
	if ( rollDelta > 0 )
	{
		if ( newAngle > localRollMax )
		{
			rollDelta = (localRollMax - this->rollAngle);
		}
	}
	else if ( rollDelta < 0 )
	{
		if ( newAngle < localRollMax )
		{
			rollDelta = (localRollMax - this->rollAngle);
		}
	}

	// Apply roll
	if ( rollDelta != 0 )
	{
		this->rollAngle += rollDelta;
		this->target->referenceFrame.rotateZ( rollDelta );
	}
}
