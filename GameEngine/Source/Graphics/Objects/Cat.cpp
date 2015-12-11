#include "Cat.h"
#include "OGLObject.h"

Cat::Cat(const std::string& name, float legsSpeed, OGLObject* body, OGLObject* head, OGLObject* ears, OGLObject* tail, 
	OGLObject* frontLeftLeg, OGLObject* frontRightLeg, OGLObject* backLeftLeg, OGLObject* backRightLeg) : OGL3DCompositeObject(name)
{
	this->legsSpeed = legsSpeed;
	this->leftLegsSpeed = legsSpeed;
	this->rightLegsSpeed = -legsSpeed;
	this->leftLegsAngleX = 0.0f;
	this->rightLegsAngleX = 0.0f;

	this->body = body;
	this->head = head;
	this->ears = ears;
	this->tail = tail;
	this->frontLeftLeg = frontLeftLeg;
	this->frontRightLeg = frontRightLeg;
	this->backLeftLeg = backLeftLeg;
	this->backRightLeg = backRightLeg;
}

Cat::~Cat()
{
	delete this->body;
	delete this->head;
	delete this->ears;
	delete this->tail;
	delete this->frontLeftLeg;
	delete this->frontRightLeg;
	delete this->backLeftLeg;
	delete this->backRightLeg;
}

void Cat::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->body->setShaderProgram(this->shaderProgram);
	this->head->setShaderProgram(this->shaderProgram);
	this->ears->setShaderProgram(this->shaderProgram);
	this->tail->setShaderProgram(this->shaderProgram);
	this->frontLeftLeg->setShaderProgram(this->shaderProgram);
	this->frontRightLeg->setShaderProgram(this->shaderProgram);
	this->backLeftLeg->setShaderProgram(this->shaderProgram);
	this->backRightLeg->setShaderProgram(this->shaderProgram);
}

void Cat::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	OGL3DCompositeObject::update(playerPosition, elapsedSeconds);

	this->leftLegsAngleX += (this->leftLegsSpeed * elapsedSeconds);
	if (this->leftLegsAngleX > 18)
		this->leftLegsSpeed = -this->legsSpeed;
	else if (this->leftLegsAngleX < -18)
		this->leftLegsSpeed = this->legsSpeed;

	this->rightLegsAngleX += (this->rightLegsSpeed * elapsedSeconds);
	if (this->rightLegsAngleX > 18)
		this->rightLegsSpeed = -this->legsSpeed;
	else if (this->rightLegsAngleX < -18)
		this->rightLegsSpeed = this->legsSpeed;
}

void Cat::render()
{
	// Body
	this->body->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->body->referenceFrame);
	this->body->render();

	// Head
	this->frameStack.push();
	{
		this->head->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Ears
	this->frameStack.push();
	{
		this->ears->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Tail
	this->frameStack.push();
	{
		this->tail->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Front left leg
	this->frameStack.push();
	{
		this->frameStack.rotateX(this->leftLegsAngleX);
		this->frontLeftLeg->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Front right leg
	this->frameStack.push();
	{
		this->frameStack.rotateX(this->rightLegsAngleX);
		this->frontRightLeg->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Back left leg
	this->frameStack.push();
	{
		this->frameStack.rotateX(this->leftLegsAngleX);
		this->backLeftLeg->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Back right leg
	this->frameStack.push();
	{
		this->frameStack.rotateX(this->rightLegsAngleX);
		this->backRightLeg->render(this->frameStack.top());
	}
	this->frameStack.pop();
}