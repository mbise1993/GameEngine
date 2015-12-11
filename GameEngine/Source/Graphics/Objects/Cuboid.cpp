#include "Cuboid.h"
#include "FlatSurface.h"

Cuboid::Cuboid(const std::string& name, float height, float width, float depth, RGBA bottomColor, RGBA topColor,
	RGBA frontColor, RGBA backColor, RGBA leftColor, RGBA rightColor) :
	OGL3DCompositeObject(name)
{
	this->height = height;
	this->width = width;
	this->depth = depth;

	this->bottom = new FlatSurface("Bottom", width, depth, bottomColor);
	this->top = new FlatSurface("Top", width, depth, topColor);
	this->front = new FlatSurface("Front", width, height, frontColor);
	this->back = new FlatSurface("Back", width, height, backColor);
	this->left = new FlatSurface("Left", depth, height, leftColor);
	this->right = new FlatSurface("Right", depth, height, rightColor);
}

Cuboid::~Cuboid()
{

}

void Cuboid::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->bottom->setShaderProgram(this->shaderProgram);
	this->top->setShaderProgram(this->shaderProgram);
	this->front->setShaderProgram(this->shaderProgram);
	this->back->setShaderProgram(this->shaderProgram);
	this->left->setShaderProgram(this->shaderProgram);
	this->right->setShaderProgram(this->shaderProgram);
}

void Cuboid::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	OGL3DCompositeObject::update(playerPosition, elapsedSeconds);
}

void Cuboid::render()
{
	render(this->referenceFrame);
}

//
// Create cuboid as a hierarchical model with the bottom as the root
// -> Bottom
//     -> Top
//	   -> Front
//        -> Back
//     -> Left
//         -> Right
//
void Cuboid::render(OGLReferenceFrame& referenceFrame)
{
	// Bottom
	this->bottom->referenceFrame = referenceFrame;
	this->frameStack.setBaseFrame(this->bottom->referenceFrame);
	this->frameStack.rotateX(180.0f);
	this->bottom->render(this->frameStack.top());

	// Top
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, -this->height, 0.0f);
		this->frameStack.rotateX(180.0f);
		this->top->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Front
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, 0.0f, -this->depth / 2);
		this->frameStack.translate(0.0f, -this->height / 2, 0.0f);
		this->frameStack.rotateX(-90.0f);
		this->front->render(this->frameStack.top());

		// Back
		this->frameStack.push();
		{
			this->frameStack.translate(0.0f, -this->depth, 0.0f);
			this->frameStack.rotateX(180.0f);
			this->back->render(this->frameStack.top());
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();

	// Left
	this->frameStack.push();
	{
		this->frameStack.translate(this->width / 2, 0.0f, 0.0f);
		this->frameStack.translate(0.0f, -this->height / 2, 0.0f);
		this->frameStack.rotateY(90.0f);
		this->frameStack.rotateX(90.0f);
		this->left->render(this->frameStack.top());

		// Right
		this->frameStack.push();
		{
			this->frameStack.translate(0.0f, -this->width, 0.0f);
			this->frameStack.rotateX(180.0f);
			this->right->render(this->frameStack.top());
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}