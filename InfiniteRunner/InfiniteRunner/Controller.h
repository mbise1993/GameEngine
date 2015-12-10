#pragma once
#ifndef GAME_LOGIC_CONTROLLER_H
#define GAME_LOGIC_CONTROLLER_H

#include "GameObject.h"
#include "OGL3DObject.h"
#include "IBehavior.h"

#include <string>

class Controller : public IBehavior
{
protected:
	OGL3DObject* target;

	float maxHealth;
	float health;

	double timeAlive;
public:
	Controller( OGL3DObject* target, float maximumHealth = 1.0f );

	virtual ~Controller();

	float getHealthPercentage();

	inline void addDamage( float amount ) { this->setHealth( this->health - amount ); }
	inline void healDamage( float amount ) { this->setHealth( this->health + amount ); }

	virtual void setHealth( float amount );
	inline float getHealth() { return this->health; }

	void setMaxHealth( float amount );
	inline float getMaxHealth() { return this->maxHealth; }

	inline double getSecondsAlive() { return this->timeAlive; }

	inline glm::vec3 getPosition() { return this->target->referenceFrame.getPosition(); }

	void update( GameObject *object, float elapsedSeconds );

	OGL3DObject* getTargetObject() { return this->target; }

protected:
	inline void moveForward( float distance ) { this->target->referenceFrame.moveForward( distance ); }
	inline void moveBackward( float distance ) { this->target->referenceFrame.moveBackward( distance ); }
	inline void moveLeft( float distance ) { this->target->referenceFrame.moveLeft( distance ); }
	inline void moveRight( float distance ) { this->target->referenceFrame.moveRight( distance ); }

};

#endif
