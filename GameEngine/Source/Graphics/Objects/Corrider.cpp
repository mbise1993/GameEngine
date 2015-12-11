#include "Corrider.h"
#include "FlatSurface.h"
#include "Cuboid.h"

Corrider::Corrider(const std::string& name, float doorwayHeight, float doorwayWidth, float corriderLength, 
	RGBA floorColor, RGBA ceilingColor, RGBA leftWallColor, RGBA rightWallColor) :
	OGL3DCompositeObject(name)
{
	this->doorwayHeight = doorwayHeight;
	this->doorwayWidth = doorwayWidth + 1;
	this->corriderLength = corriderLength;

	this->floor = new FlatSurface("Floor", this->doorwayWidth, this->corriderLength, floorColor);
	this->ceiling = new FlatSurface("Ceiling", this->doorwayWidth, this->corriderLength, ceilingColor);
	this->leftWall = new Cuboid("LeftWall", 1.0f, this->doorwayHeight, this->corriderLength, 
		leftWallColor, leftWallColor, leftWallColor, leftWallColor, leftWallColor, leftWallColor);
	this->rightWall = new Cuboid("RightWall", 1.0f, this->doorwayHeight, this->corriderLength, 
		rightWallColor, rightWallColor, rightWallColor, rightWallColor, rightWallColor, rightWallColor);
}

Corrider::~Corrider()
{
	delete this->floor;
	delete this->ceiling;
	delete this->leftWall;
	delete this->rightWall;
}

void Corrider::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->floor->setShaderProgram(this->shaderProgram);
	this->ceiling->setShaderProgram(this->shaderProgram);
	this->leftWall->setShaderProgram(this->shaderProgram);
	this->rightWall->setShaderProgram(this->shaderProgram);
}

void Corrider::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	OGL3DCompositeObject::update(playerPosition, elapsedSeconds);
}

//
// Create hallway as a hierarchical model with the floor as the root
// -> Floor
//     -> Ceiling
//     -> Left wall
//         -> Right wall
//
void Corrider::render()
{
	// Floor
	this->floor->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->floor->referenceFrame);
	this->floor->render();

	// Ceiling
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, this->doorwayHeight, 0.0f);
		this->frameStack.rotateX(180.0f);
		this->ceiling->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Left wall
	this->frameStack.push();
	{
		this->frameStack.translate(-(this->doorwayWidth + 1.0f) / 2, 0.0f, 0.0f);
		this->frameStack.translate(0.0f, this->doorwayHeight / 2, 0.0f);
		this->frameStack.rotateZ(-90.0f);
		this->leftWall->render(this->frameStack.top());

		// Right wall
		this->frameStack.push();
		{
			this->frameStack.translate(0.0f, this->doorwayWidth, 0.0f);
			this->rightWall->render(this->frameStack.top());
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}