#include "GameObject.h"
#include "IBehavior.h"

using std::map;
using std::string;

GameObject::GameObject( string name )
{
	this->name = name;
	this->visible = false;
	this->behavior = nullptr;
	this->material.ambientIntensity = 0.1f;
	this->material.specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->material.shininess = 0.000001f;
	this->texture = nullptr;
	this->showBoundingBox = false;
}

void GameObject::update( float elapsedSeconds )
{
	if ( this->behavior )
	{
		this->behavior->update( this, elapsedSeconds );
	}
}

GameObject::~GameObject()
{
	if ( this->behavior )
	{
		delete this->behavior;
	}
}

bool GameObject::checkForCollision( GameObject* otherObject )
{
	// Can't collide with self
	if ( this->getName() != otherObject->getName() )
	{
		if ( !this->isCollidingWith( otherObject ) )
		{
			if ( this->boundingBox.overlapsWith( otherObject->boundingBox ) )
			{
				this->objectsColliding[otherObject->getName()] = otherObject;
				otherObject->objectsColliding[this->getName()] = this;
				return true;
			}
		}
	}
	return false;
}

bool GameObject::isCollidingWith( GameObject* otherObject )
{
	return this->objectsColliding.find( otherObject->getName() ) != this->objectsColliding.end();
}
