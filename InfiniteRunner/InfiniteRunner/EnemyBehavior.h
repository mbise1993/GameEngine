#pragma once
#ifndef GAME_GRAPHICS_ENEMY_BEHAVIOR_H
#define GAME_GRAPHICS_ENEMY_BEHAVIOR_H

#include "IBehavior.h"
#include "DestroyerMissile.h"

#include <glm\glm.hpp>

class PlayerController;
class OGL3DGroupObject;

class EnemyBehavior : 
	public IBehavior
{
private:
	PlayerController* player;

	OGL3DGroupObject* amo;

	DestroyerMissile* amoBehavior;

	float xAcceleration;

	float maxAcceleration;

	float speed;

public:

	EnemyBehavior( PlayerController* player, OGL3DGroupObject* amo );

	virtual ~EnemyBehavior();

	void update( GameObject *object, float elapsedSeconds );
};
#endif
