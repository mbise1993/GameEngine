#pragma once
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "BaseObject.h"
#include "RGBA.h"
#include "BoundingBox.h"

#include <string>
#include <map>

class IBehavior;
class Texture;

struct Material
{
	float ambientIntensity; // Light emitted by this object
	RGBA specular;
	float shininess;
};

// GameObject -> BaseObject
class GameObject : public BaseObject
{
protected:

	bool visible;

	IBehavior* behavior;

	Texture* texture;

	std::map<std::string, GameObject*> objectsColliding;

public:
	Material material;

	BoundingBox boundingBox;

	bool showBoundingBox;

public:
	GameObject( std::string name );

	virtual ~GameObject();

	virtual void setTexture( Texture* texture ) { this->texture = texture; }

	bool isTextured() { return this->texture != nullptr; }

	virtual void update( float elapsedSeconds );

	virtual void render() = 0;

	void setVisibility( bool visible ) { this->visible = visible; }

	bool isVisible() const { return this->visible; }

	void setBehavior( IBehavior* behavior ) { this->behavior = behavior; }

	bool checkForCollision( GameObject* otherObject );

	bool isCollidingWith( GameObject* otherObject );

	bool isColliding() { return this->objectsColliding.size() > 0; }

	void clearCollisions() { this->objectsColliding.clear(); }
};

#endif

