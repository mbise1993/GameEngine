#include "EnemyBehavior.h"
#include "PlayerController.h"
#include "OGL3DGroupObject.h"

EnemyBehavior::EnemyBehavior( PlayerController * player, OGL3DGroupObject* amo )
{
	this->player = player;
	this->amo = amo;
	this->xAcceleration = 0.0f;
	this->speed = 10.0f;
	this->maxAcceleration = this->speed * 1.544f;

	this->amoBehavior = new DestroyerMissile( player );
	this->amo->setBehavior( this->amoBehavior );
}

EnemyBehavior::~EnemyBehavior()
{}

void EnemyBehavior::update( GameObject * object, float elapsedSeconds )
{
	OGL3DObject* enemy = (OGL3DObject*)object;

	// Rotate
	enemy->referenceFrame.rotateY( 180.0f * elapsedSeconds );

	if ( this->player->isGameOver() ) return;

	glm::vec3 playerPosition = this->player->getPosition();
	glm::vec3 enemyPosition = enemy->referenceFrame.getPosition();

	// X acceleration
	if ( playerPosition.x > enemyPosition.x )
	{
		this->xAcceleration += this->speed * elapsedSeconds;
		if ( this->xAcceleration > this->maxAcceleration ) this->xAcceleration = this->maxAcceleration;

	}
	else if ( playerPosition.x < enemyPosition.x )
	{
		this->xAcceleration -= this->speed * elapsedSeconds;
		if ( this->xAcceleration < -this->maxAcceleration ) this->xAcceleration = -this->maxAcceleration;
	}

	// Y offset
	enemyPosition.y = (sinf( (float)this->player->getSecondsAlive() ) - 0.5f);

	// Make sure the enemy stays with the player on the Z axis
	enemyPosition.z = playerPosition.z + 8.0f;

	// Adjust X
	enemyPosition.x = ((enemyPosition.x + (this->xAcceleration * elapsedSeconds)) + (enemyPosition.x)) / 2.0f;

	// Set position
	enemy->referenceFrame.setPosition( enemyPosition );

	// Missile
	if ( !this->amoBehavior->isMissileActive() )
	{
		if ( abs( enemyPosition.x - playerPosition.x ) < 10.0f )
		{
			this->amoBehavior->setActive( enemyPosition );
		}
	}
}
