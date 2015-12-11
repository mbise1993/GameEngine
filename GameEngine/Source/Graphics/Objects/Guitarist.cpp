#include "Guitarist.h"
#include "OGLObject.h"

Guitarist::Guitarist(const std::string& name, float strumSpeed, OGLObject* torso, OGLObject* legs, OGLObject* head,
	OGLObject* leftArm, OGLObject* rightBicep, OGLObject* rightForearm) : OGL3DCompositeObject(name)
{
	this->strumSpeed = strumSpeed;
	this->currentStrumSpeed = strumSpeed;
	this->strumAngleZ = 0.0f;

	this->torso = torso;
	this->legs = legs;
	this->head = head;
	this->leftArm = leftArm;
	this->rightBicep = rightBicep;
	this->rightForearm = rightForearm;
	this->rightForearmCuboid = new Cuboid("RightForearmCuboid", 0.31f, 1.4f, 0.26f,
		{ 0.246f, 0.246f, 0.246f, 1.0f },
		{ 0.246f, 0.246f, 0.246f, 1.0f },
		{ 0.246f, 0.246f, 0.246f, 1.0f },
		{ 0.246f, 0.246f, 0.246f, 1.0f },
		{ 0.246f, 0.246f, 0.246f, 1.0f },
		{ 0.246f, 0.246f, 0.246f, 1.0f });
}

Guitarist::~Guitarist()
{
	delete this->torso;
	delete this->legs;
	delete this->head;
	delete this->leftArm;
	delete this->rightBicep;
	delete this->rightForearm;
	delete this->rightForearmCuboid;
}

void Guitarist::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->torso->setShaderProgram(this->shaderProgram);
	this->legs->setShaderProgram(this->shaderProgram);
	this->head->setShaderProgram(this->shaderProgram);
	this->leftArm->setShaderProgram(this->shaderProgram);
	this->rightBicep->setShaderProgram(this->shaderProgram);
	this->rightForearm->setShaderProgram(this->shaderProgram);
	this->rightForearmCuboid->setShaderProgram(this->shaderProgram);
}

void Guitarist::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	OGL3DCompositeObject::update(playerPosition, elapsedSeconds);

	this->strumAngleZ += (this->currentStrumSpeed * elapsedSeconds);
	if (this->strumAngleZ > 10)
		this->currentStrumSpeed = -this->strumSpeed;
	else if (this->strumAngleZ < -10)
		this->currentStrumSpeed = this->strumSpeed;
}

void Guitarist::render()
{
	// Body
	this->torso->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->torso->referenceFrame);
	this->torso->render();

	// Legs
	this->frameStack.push();
	{
		this->legs->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Head
	this->frameStack.push();
	{
		this->head->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Left arm
	this->frameStack.push();
	{
		this->leftArm->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Right bicep
	this->frameStack.push();
	{
		this->rightBicep->render(this->frameStack.top());
	}
	this->frameStack.pop();

	// Right forearm
	/*this->frameStack.push();
	{
		this->frameStack.translate(0.0f, -0.8f, 0.0f);
		this->frameStack.translate(-2.1f, 0.0f, 0.0f);
		this->frameStack.rotateZ(this->strumAngleZ);
		this->frameStack.translate(2.1f, 0.0f, 0.0f);
		this->frameStack.translate(0.0f, 0.8f, 0.0f);
		this->rightForearm->render(this->frameStack.top());
	}
	this->frameStack.pop();*/

	// Right forearm cuboid
	this->frameStack.push();
	{
		this->frameStack.translate(0.0f, 1.2f, 0.0f);
		this->frameStack.translate(0.0f, 0.0f, 1.3f);
		this->frameStack.translate(-1.6f, 0.0f, 0.0f);
		this->frameStack.rotateZ(this->strumAngleZ);
		this->frameStack.translate(0.7f, 0.0f, 0.0f);
		this->rightForearmCuboid->render(this->frameStack.top());
	}
	this->frameStack.pop();
}