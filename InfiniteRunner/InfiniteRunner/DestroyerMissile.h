#pragma once
#ifndef GAME_GRAPHICS_MISSILE_BEHAVIOR_H
#define GAME_GRAPHICS_MISSILE_BEHAVIOR_H

#include "IBehavior.h"
#include "OGLReferenceFrame.h"

#include <glm\glm.hpp>

class PlayerController;
class OGLReferenceFrame;

class DestroyerMissile :
	public IBehavior
{
private:
	PlayerController* player;

	OGLReferenceFrame frame;

	glm::vec3 startingPosition;

	bool justActivated;

	bool isActive;

public:
	DestroyerMissile( PlayerController* player );

	virtual ~DestroyerMissile();

	void update( GameObject *object, float elapsedSeconds );

	void setActive( glm::vec3 position );

	void setInactive();

	inline bool isMissileActive() { return this->isActive; }

private:
	float closeDistance( float value, float speed );
};
#endif
