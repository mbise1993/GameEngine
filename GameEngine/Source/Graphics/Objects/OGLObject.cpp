#include "OGLObject.h"
#include "Lighting/LightSource.h"

#ifndef NULL
#define NULL 0
#endif
#include <glm/gtc/type_ptr.hpp>

OGLObject::OGLObject(string name) : GameObject(name)
{
	this->vao = 0;
	this->setArrayBufferType();
}

OGLObject::~OGLObject()
{
	if (this->lightSource)
		delete this->lightSource;

	for (auto iterator = this->objects.begin(); iterator != this->objects.end(); iterator++) {
		delete iterator->second;
	}
	glDeleteVertexArrays(1, &this->vao);
}

void OGLObject::addVBOObject(VBOObject * object)
{
	if (this->vao == 0) {
		glGenVertexArrays(1, &this->vao);
	}
	glBindVertexArray(this->vao);
	object->vbo = createBufferObject(
		GL_ARRAY_BUFFER, 
		object->buffer, 
		object->bufferSizeInBytes);

	if (this->vboType == INDEXED_ARRAY) {
		object->ibo = createBufferObject(
			GL_ELEMENT_ARRAY_BUFFER, object->indexData, object->indexSizeInBytes);
	}
	
	this->objects[object->name] = object;
	glBindVertexArray(0);
}

void OGLObject::setLightSource(LightSource* lightSource)
{
	Vector3f pos = lightSource->getPosition();
	lightSource->setPosition(
		pos.v1 + this->referenceFrame.getPosition()[0], 
		pos.v2 + this->referenceFrame.getPosition()[1],
		pos.v3 + this->referenceFrame.getPosition()[2]);

	this->lightSource = lightSource;
}

void OGLObject::update(glm::vec3 playerPosition, float elapsedSeconds)
{
	// If object has a light source, update its position and on/off
	if(this->lightSource != NULL)
	{
		// If player is within 3 units of the light source in the XZ plane, turn it on/off
		if (shouldUpdateLight(playerPosition, 3.0f))
		{
			if (this->lightSource->getState() == LIGHT_ON)
				this->lightSource->setState(LIGHT_OFF);
			else
				this->lightSource->setState(LIGHT_ON);
		}
	}

	GameObject::update(playerPosition, elapsedSeconds);
}

bool OGLObject::shouldUpdateLight(glm::vec3& playerPosition, float maxDistance)
{
	// If player was previously outside the bounds...
	if(abs(this->previousPlayerPos[0] - this->lightSource->getPosition().v1) > maxDistance ||
	   abs(this->previousPlayerPos[2] - this->lightSource->getPosition().v3) > maxDistance)
	{
		// ...and is now inside the bounds, update light
		if (abs(playerPosition[0] - this->lightSource->getPosition().v1) < maxDistance &&
			abs(playerPosition[2] - this->lightSource->getPosition().v3) < maxDistance)
		{
			return true;
		}
	}

	return false;
}

VBOObject * OGLObject::createVBOObject(string name, float* vertexData, GLenum primitiveType)
{
	VBOObject * object = new VBOObject();
	object->name = name;
	object->vbo = 0;
	object->primitiveType = primitiveType;
	object->buffer = &vertexData[1];
	object->bufferSizeInBytes = (unsigned int)vertexData[0] * sizeof(float);
	object->numberOfVertices = (unsigned int)vertexData[0] / sizeof(float) / 2;
	object->indexData = NULL;
	object->indexSizeInBytes = 0;
	object->numberOfIndexes = 0;
	object->positionComponent.type = GL_FLOAT;
	object->positionComponent.count = 4;
	object->positionComponent.bytesToFirst = 0;
	object->positionComponent.bytesToNext = object->positionComponent.count * sizeof(float);
	object->colorComponent.type = GL_FLOAT;
	object->colorComponent.count = 4;
	object->colorComponent.bytesToFirst = 
		sizeof(float) * object->numberOfVertices * object->positionComponent.count;
	object->colorComponent.bytesToNext = object->colorComponent.count * sizeof(float);
	return object;
}

VBOObject* OGLObject::createVBOObject(string name, float* vertexData, short* indexData, GLenum primitiveType)
{
	VBOObject * object = new VBOObject();
	object->positionComponent.count = 4;
	object->colorComponent.count = 4;
	object->normalComponent.count = 3;

	object->name = name;
	object->vbo = 0;
	object->primitiveType = primitiveType;
	object->buffer = &vertexData[1];
	object->bufferSizeInBytes = (unsigned int)vertexData[0] * sizeof(float);
	object->numberOfVertices = (unsigned int)vertexData[0] /
		(object->positionComponent.count + object->colorComponent.count + object->normalComponent.count);
	object->indexData = &indexData[1];
	object->numberOfIndexes = indexData[0];
	object->indexSizeInBytes = object->numberOfIndexes * sizeof(short);
	// Positions
	object->positionComponent.type = GL_FLOAT;
	object->positionComponent.bytesToFirst = 0;
	object->positionComponent.bytesToNext = 0;
	// Colors
	object->colorComponent.type = GL_FLOAT;
	object->colorComponent.bytesToFirst =
		sizeof(float) * object->positionComponent.count * object->numberOfVertices;
	object->colorComponent.bytesToNext = 0;
	// Normals
	object->normalComponent.type = GL_FLOAT;
	object->normalComponent.bytesToFirst =
		sizeof(float) * (object->positionComponent.count + object->colorComponent.count) * object->numberOfVertices;
	object->normalComponent.bytesToNext = 0;
	return object;
}

VBOObject* OGLObject::createVBOObject(string name, std::vector<Vertex>* vertexData, GLenum primitiveType, Material material)
{
	VBOObject * object = new VBOObject();
	object->positionComponent.count = 4;
	object->colorComponent.count = 4;
	object->normalComponent.count = 3;

	object->name = name;
	object->vbo = 0;
	object->materialData = material;
	object->primitiveType = primitiveType;
	object->buffer = vertexData->data();
	object->bufferSizeInBytes = vertexData->size() * sizeof(Vertex);
	object->numberOfVertices = vertexData->size();
	// Positions
	object->positionComponent.type = GL_FLOAT;
	object->positionComponent.bytesToFirst = 0;
	object->positionComponent.bytesToNext = sizeof(Vertex);
	// Colors
	object->colorComponent.type = GL_FLOAT;
	object->colorComponent.bytesToFirst = sizeof(float) * (object->positionComponent.count + object->normalComponent.count);
	object->colorComponent.bytesToNext = sizeof(Vertex);
	// Normals
	object->normalComponent.type = GL_FLOAT;
	object->normalComponent.bytesToFirst = sizeof(float) * object->positionComponent.count;
	object->normalComponent.bytesToNext = sizeof(Vertex);
	
	return object;
}

VBOObject* OGLObject::getVBOObject(string name)
{
	if (this->objects.find(name) != this->objects.end()) {
		return this->objects[name];
	}
	return NULL;
}

GLuint OGLObject::createBufferObject(GLenum target, const void* bufferData, GLsizei bufferSize)
{
	GLuint bufferObjectID = 0;
	glGenBuffers(1, &bufferObjectID);
	glBindBuffer(target, bufferObjectID);
	glBufferData(target, bufferSize, bufferData, GL_STATIC_DRAW);
	glBindBuffer(target, 0);
	return bufferObjectID;
}

GLuint OGLObject::createTextureObject(GLenum target, GLenum colorFormat, GLuint width, GLuint height, const void* bufferData)
{
	GLuint textureObjectID = 0;
	glGenTextures(1, &textureObjectID);
	glBindTexture(target, textureObjectID);

	if (target == GL_TEXTURE_2D)
		glTexImage2D(target, 0, GL_RGB, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, bufferData);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(target, 0);
	return textureObjectID;
}

void OGLObject::renderVboObjects()
{
	auto iterator = this->objects.begin();
	while (iterator != this->objects.end()) 
	{
		this->sendMaterialDataToGPU(iterator->second->materialData);
		this->renderObject(iterator->second);
		iterator++;
	}
}

void OGLObject::renderObject(VBOObject* object)
{
	glBindVertexArray(this->vao);
	glUseProgram(this->shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, object->vbo);

	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		object->positionComponent.count,
		object->positionComponent.type,
		GL_FALSE,
		object->positionComponent.bytesToNext,
		(void*)object->positionComponent.bytesToFirst);

	// Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		object->colorComponent.count,
		object->colorComponent.type,
		GL_FALSE,
		object->colorComponent.bytesToNext,
		(void*)object->colorComponent.bytesToFirst);

	// Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		object->normalComponent.count,
		object->normalComponent.type,
		GL_FALSE,
		object->normalComponent.bytesToNext,
		(void*)object->normalComponent.bytesToFirst);

	if (this->vboType == INDEXED_ARRAY) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
		glDrawElements(object->primitiveType, object->numberOfIndexes, GL_UNSIGNED_SHORT, 0);
	}
	else {
		glDrawArrays(object->primitiveType, 0, object->numberOfVertices);
	}
	

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
	glBindVertexArray(0);
}

void OGLObject::sendMaterialDataToGPU(const Material material)
{
	auto ambientLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialAmbientIntensity");
	auto shininessLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialShininess");
	auto specularLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialSpecular");

	glUseProgram(this->shaderProgram);
	glUniform1f(ambientLocation, material.ambientIntensity);
	glUniform1f(shininessLocation, material.specularExp);
	glm::vec4 vector =
		glm::vec4(
			material.specularColor.red,
			material.specularColor.green,
			material.specularColor.blue,
			material.specularColor.alpha);
	glUniform4fv(specularLocation, 1, glm::value_ptr(vector));
	glUseProgram(0);
}