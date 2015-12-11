#include "OGLGraphicsSystem.h"
#include "Shaders/OGLShaderManager.h"
#include "Shaders/OGLVertexShader.h"
#include "Shaders/OGLFragmentShader.h"
#include "Shaders/OGLShaderProgram.h"
#include "GameWorld.h"
#include "OGLViewingFrustum.h"
#include "Logger.h"
#include "Cameras/OGLCamera.h"
#include "Objects/GameObjectManager.h"
#include "Objects/OGLObject.h"
#include "Lighting/LightSource.h"
#include "TheGame.h"

#include <map>
using std::map;
#include <glm\gtc\type_ptr.hpp>
#include <glutil\glutil.h>

OGLGraphicsSystem::OGLGraphicsSystem(
	OGLShaderManager *shaderManager, 
	GameWorld *gameWorld, 
	OGLViewingFrustum *oglFrustum) :
		GraphicsSystem(shaderManager, gameWorld, oglFrustum)
{
	this->oglShaderManager = (OGLShaderManager*)this->shaderManager;
	this->oglFrustum = (OGLViewingFrustum*)this->frustum;
	this->oglCamera = (OGLCamera*)this->gameWorld->getCamera();
}

OGLGraphicsSystem::~OGLGraphicsSystem()
{
}

void OGLGraphicsSystem::addVertexShader(
	const string& name, OGLVertexShader *shader, const string& sourceCode)
{
	if (sourceCode != "") {
		shader->setSource(sourceCode);
	}
	shader->setLogger(this->logger);
	this->oglShaderManager->addVertexShader(name, shader);
}

void OGLGraphicsSystem::addFragmentShader(
	const string& name, OGLFragmentShader *shader, const string& sourceCode)
{
	if (sourceCode != "") {
		shader->setSource(sourceCode);
	}
	shader->setLogger(this->logger);
	this->oglShaderManager->addFragmentShader(name, shader);
}

void OGLGraphicsSystem::addShaderProgram(
	const string& name,
	OGLShaderProgram *shaderProgram, const string& vertexShaderName, const string& fragmentShaderName)
{
	shaderProgram->setLogger(this->logger);
	this->oglShaderManager->addShaderProgram(
		name, shaderProgram, vertexShaderName, fragmentShaderName);
}

bool OGLGraphicsSystem::initialize()
{
	this->initialized = this->oglShaderManager->createShaders();
	return this->initialized;
}

void OGLGraphicsSystem::setUpViewingEnvironment()
{
	this->theGame->setupViewingEnvironment();
}

void OGLGraphicsSystem::clearViewPort(
	unsigned int left, unsigned int top,
	unsigned int width, unsigned int height,
	RGBA color)
{
	glViewport(left, top, width, height);
	glClearColor(color.red, color.green, color.blue, color.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OGLGraphicsSystem::setObjectsShaderProgram(const string& shaderProgramName)
{
	GLuint shaderProgram =
		this->oglShaderManager->getShaderHandle(shaderProgramName);
	auto allObjects = this->getGameWorld()->getObjectManager()->getAllObjects();

	OGLObject *object;
	auto iterator = allObjects.begin();
	while (iterator != allObjects.end()) {
		object = (OGLObject*)iterator->second;
		object->setShaderProgram(shaderProgram);
		iterator++;
	}
}

void OGLGraphicsSystem::setObjectShaderProgram(const string& objectName, const string& shaderProgramName)
{
	GLuint shaderProgram =
		this->oglShaderManager->getShaderHandle(shaderProgramName);
	GameObjectManager *objectMgr = this->gameWorld->getObjectManager();
	OGLObject *object = (OGLObject *)objectMgr->getObject(objectName);
	object->setShaderProgram(shaderProgram);
}

void OGLGraphicsSystem::update(float elapsedSeconds)
{
	GraphicsSystem::update(elapsedSeconds);
}

void OGLGraphicsSystem::render()
{
	this->theGame->sendShaderData();

	GameObjectManager *objectMgr = this->gameWorld->getObjectManager();
	objectMgr->renderVisibleObjects();
}
