#pragma once
#ifndef GAME_LOGIC_PLAYER_CONTROLLER_H
#define GAME_LOGIC_PLAYER_CONTROLLER_H

#include "Controller.h"
#include "Subscriber.h"
#include <string>

class PCInputSystem;
class ChunkSystem;

class PlayerController :
	public Controller,
	public Subscriber
{
private:
	enum StrafeDirections
	{
		None, Left, Right
	} strafeDirection;

	float rollSpeed;
	float rollAngle;
	float rollMax;

	bool gameOver;

public:
	float speed;
	float accelerationPerSecond;
	float startAcclerationPerSecond;
	float strafeSpeed;

	float startingSpeed;
	glm::vec3 startingPosition;

public:
	PlayerController( OGL3DObject* playerObject,
		float maximumHealth, float strafeSpeed,
		float acclerationPerSecond, float startingSpeed );

	virtual ~PlayerController();

	inline void setStrafeSpeed( float amount ) { this->strafeSpeed = amount; }
	inline float getStrafeSpeed() { return this->strafeSpeed; }

	inline void setAccelerationPS( float acceleration ) { this->accelerationPerSecond = acceleration; }
	inline float getAcclerationPS() { return this->accelerationPerSecond; }

	void setHealth( float amount );
	void setStartingPosition( glm::vec3 position );
	inline float getCurrentSpeed() { return this->speed; }

	void processInputs( PCInputSystem* inputSystem );
	void update( GameObject * object, float elapsedSeconds );
	void checkForCollisions( ChunkSystem* chunkSystem );

	void resetPlayer();
	void pausePlayer();

	inline bool isGameOver() { return this->gameOver; }

	inline int getScore() { return (int)(this->timeAlive * 50.3f); }

	inline OGL3DObject* getPlayerObject() { return this->target; }

	void eventHandler( EventArgs* eventArgs );

private:
	void performStrafeAndRoll( float elapsedSeconds );
};
#endif
