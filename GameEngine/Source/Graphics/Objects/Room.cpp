#include "Room.h"
#include "FlatSurface.h"
#include "Cuboid.h"
#include "Lighting/LightSource.h"

Room::Room(const std::string& name, float width, float height, float depth, float doorHeight, float doorWidth,
	RGBA floorColor, RGBA ceilingColor, RGBA frontWallColor, RGBA backWallColor, RGBA leftWallColor, RGBA rightWallColor) :
	OGL3DCompositeObject(name)
{
	this->width = width;
	this->depth = depth;
	this->height = height;
	this->doorHeight = doorHeight;
	this->doorWidth = doorWidth;

	this->floor = new FlatSurface("Floor", width, depth, floorColor);
	this->ceiling = new FlatSurface("Ceiling", width, depth, ceilingColor);
	this->frontWallTop = new Cuboid("FrontWallTop", 1.0f, width, height - doorHeight, frontWallColor, frontWallColor, 
		frontWallColor, frontWallColor, frontWallColor, frontWallColor);
	this->frontWallLeft = new Cuboid("FrontWallLeft", 1.0f, (width / 2) - (doorWidth / 2), doorHeight, frontWallColor,
		frontWallColor, frontWallColor, frontWallColor, frontWallColor, frontWallColor);
	this->frontWallRight = new Cuboid("FrontWallRight", 1.0f, (width / 2) - (doorWidth / 2), doorHeight, frontWallColor,
		frontWallColor, frontWallColor, frontWallColor, frontWallColor, frontWallColor);
	this->backWall = new Cuboid("BackWall", 1.0f, width, height, backWallColor, backWallColor, backWallColor,
		backWallColor, backWallColor, backWallColor);
	this->leftWall = new Cuboid("LeftWall", 1.0f, depth, height, leftWallColor, leftWallColor, leftWallColor,
		leftWallColor, leftWallColor, leftWallColor);
	this->rightWall = new Cuboid("RightWall", 1.0f, depth, height, rightWallColor, rightWallColor, rightWallColor,
		rightWallColor, rightWallColor, rightWallColor);
}

Room::~Room()
{
	delete this->floor;
	delete this->ceiling;
	delete this->frontWallTop;
	delete this->frontWallLeft;
	delete this->frontWallRight;
	delete this->backWall;
	delete this->leftWall;
	delete this->rightWall;
}

void Room::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->floor->setShaderProgram(this->shaderProgram);
	this->ceiling->setShaderProgram(this->shaderProgram);
	this->frontWallTop->setShaderProgram(this->shaderProgram);
	this->frontWallLeft->setShaderProgram(this->shaderProgram);
	this->frontWallRight->setShaderProgram(this->shaderProgram);
	this->backWall->setShaderProgram(this->shaderProgram);
	this->leftWall->setShaderProgram(this->shaderProgram);
	this->rightWall->setShaderProgram(this->shaderProgram);
}

//
// Add an object to the otherObjects map
//
void Room::addOwnedObject(std::string& name, OGLObject* object)
{
	// If the object is not there then add it
	if (this->ownedObjects.find(name) == this->ownedObjects.end())
		this->ownedObjects[name] = object;
}

void Room::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	LightSource* lightSource;
	if(playerEnteredRoom(playerPosition))
	{
		for(auto object : this->ownedObjects)
		{
			lightSource = object.second->getLightSource();
			if (lightSource != NULL)
				lightSource->setState(LIGHT_ON);
		}
	}

	if(playerExitedRoom(playerPosition))
	{
		for(auto object : this->ownedObjects)
		{
			lightSource = object.second->getLightSource();
			if (lightSource != NULL)
				lightSource->setState(LIGHT_OFF);
		}
	}

	OGL3DCompositeObject::update(playerPosition, elapsedSeconds);
}

bool Room::playerEnteredRoom(glm::vec3& playerPosition)
{
	// If player was outside the bounds of the room...
	if(!isBetween(this->frontWallTop->getPosition()[2], this->backWall->getPosition()[2], this->previousPlayerPos[2]) ||
	   !isBetween(this->leftWall->getPosition()[0], this->rightWall->getPosition()[0], this->previousPlayerPos[0]))
	{
		// ...and is now inside, return true
		if (isBetween(this->frontWallTop->getPosition()[2], this->backWall->getPosition()[2], playerPosition[2]) &&
			isBetween(this->leftWall->getPosition()[0], this->rightWall->getPosition()[0], playerPosition[0]))
		{
			return true;
		}
	}

	return false;
}

bool Room::playerExitedRoom(glm::vec3& playerPosition)
{
	// If player was inside the bounds of the room...
	if (isBetween(this->frontWallTop->getPosition()[2], this->backWall->getPosition()[2], this->previousPlayerPos[2]) &&
		isBetween(this->leftWall->getPosition()[0], this->rightWall->getPosition()[0], this->previousPlayerPos[0]))
	{
		// ...and is now outside, return true
		if (!isBetween(this->frontWallTop->getPosition()[2], this->backWall->getPosition()[2], playerPosition[2]) ||
			!isBetween(this->leftWall->getPosition()[0], this->rightWall->getPosition()[0], playerPosition[0]))
		{
			return true;
		}
	}

	return false;
}

//
// Create room as a hierarchical model with the floor as the root
// -> Floor
//     -> Ceiling
//         -> Front wall top
//	   -> Front wall left
//     -> Front wall right
//     -> Back wall
//     -> Left wall
//         -> Right wall
//     -> All other objects
//
void Room::render()
{
	// Floor
	this->floor->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->floor->referenceFrame);
	this->floor->render();

	// Ceiling
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, this->height, 0.0f);
		this->frameStack.rotateX(180.0f);
		this->ceiling->referenceFrame = this->frameStack.top();
		this->ceiling->render();

		// Front wall top
		this->frameStack.push();
		{
			this->frameStack.translate(0.0f, 0.0f, -this->depth / 2);
			this->frameStack.translate(0.0f, (this->height - this->doorHeight) / 2, 0.0f);
			this->frameStack.rotateX(90.0f);
			this->frontWallTop->referenceFrame = this->frameStack.top();
			this->frontWallTop->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();

	// Front wall left
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, 0.0f, this->depth / 2);
		this->frameStack.translate(0.0f, this->doorHeight / 2, 0.0f);
		this->frameStack.translate(-this->doorWidth / 2, 0.0f, 0.0f);
		this->frameStack.translate(-((this->width / 2) - (this->doorWidth / 2)) / 2, 0.0f, 0.0f);
		this->frameStack.rotateX(-90.0f);
		this->frontWallLeft->referenceFrame = this->frameStack.top();
		this->frontWallLeft->render();

		// Front wall right
		this->frameStack.push();
		{
			this->frameStack.translate((this->width / 2) + (this->doorWidth / 2), 0.0f, 0.0f);
			this->frontWallRight->referenceFrame = this->frameStack.top();
			this->frontWallRight->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();

	// Back wall
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f,0.0f, -this->depth / 2);
		this->frameStack.translate(0.0f, this->height / 2, 0.0f);
		this->frameStack.rotateX(90.0f);
		this->backWall->referenceFrame = this->frameStack.top();
		this->backWall->render();
	}
	this->frameStack.pop();

	// Left wall
	this->frameStack.push();
	{
		this->frameStack.translate(-this->width / 2, 0.0f, 0.0f);
		this->frameStack.translate(0.0f, this->height / 2, 0.0f);
		this->frameStack.rotateY(90.0f);
		this->frameStack.rotateX(90.0f);
		this->leftWall->referenceFrame = this->frameStack.top();
		this->leftWall->render();

		// Right wall
		this->frameStack.push();
		{
			this->frameStack.translate(0.0f, this->width, 0.0f);
			this->frameStack.rotateX(180.0f);
			this->rightWall->referenceFrame = this->frameStack.top();
			this->rightWall->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}