#pragma once
#ifndef IBEHAVIOR
#define IBEHAVIOR

class GameObject;

class IBehavior
{
public:
	IBehavior();
	virtual ~IBehavior();

	virtual void update(GameObject *object, float elapsedSeconds) = 0;
};

#endif

