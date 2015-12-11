#include "GameObject.h"
#include "Animation/IBehavior.h"
#include "Lighting/LightSource.h"

#ifndef NULL
#define NULL 0
#endif

GameObject::GameObject(string name)
{
	this->name = name;
	this->visible = true;
	this->vertexData = NULL;
	this->indexData = NULL;
	this->behavior = NULL;
	this->lightSource = NULL;
}

GameObject::~GameObject()
{
	if (this->vertexData != NULL)
		delete this->vertexData;
	
	if (this->indexData)
		delete[] this->indexData;
	
	if (this->behavior)
		delete this->behavior;
}

void GameObject::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	if (this->behavior) 
			this->behavior->update(this, elapsedSeconds);

	this->previousPlayerPos = playerPosition;
}

void GameObject::setBehavior(IBehavior* behavior)
{
	this->behavior = behavior;
}
