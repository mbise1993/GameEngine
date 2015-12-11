#pragma once
#ifndef OGL_OBJECT
#define OGL_OBJECT

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <map>
using std::map;
#include <string>
using std::string;
#include <vector>

#include "Vertex.h"
#include "GameObject.h"
#include "OGLReferenceFrame.h"
#include "Textures/Texture.h"
#include "Material.h"

struct Component {
	unsigned int count;
	GLenum type;
	unsigned int bytesToNext;
	unsigned int bytesToFirst;
};

struct VBOObject {
	string name;
	GLuint vbo;
	GLuint ibo;
	Material materialData;
	Texture textureData;
	GLenum primitiveType;
	void * buffer;
	unsigned int bufferSizeInBytes;
	unsigned int numberOfVertices;
	void * indexData;
	unsigned int indexSizeInBytes;
	unsigned int numberOfIndexes;
	Component positionComponent;
	Component normalComponent;
	Component textureComponent;
	Component colorComponent;
};

enum VBOType {ARRAY_BUFFER, INDEXED_ARRAY};

class OGLObject : public GameObject
{
public:
	GLuint vao;
	OGLReferenceFrame referenceFrame;

protected:
	map<string, VBOObject*> objects;
	VBOType vboType;
	glm::vec3 speed;
	GLuint shaderProgram;
	
public:
	OGLObject(string name);
	virtual ~OGLObject();

	glm::vec3 getPosition() { return this->referenceFrame.getPosition(); }

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render() = 0;
	virtual void render(OGLReferenceFrame& frame) = 0;

	void setArrayBufferType() { this->vboType = ARRAY_BUFFER; }
	void setIndexedArrayType() { this->vboType = INDEXED_ARRAY; }

	virtual void setShaderProgram(GLuint shaderProgram) {
		this->shaderProgram = shaderProgram;
	}

	void addVBOObject(VBOObject * object);
	VBOObject* getVBOObject(string name);

	void setLightSource(LightSource* lightSource);

	static VBOObject * createVBOObject(string name, float* vertexData, GLenum primitiveType);
	static VBOObject * createVBOObject(string name, float* vertexData, short* indexData, GLenum primitiveType = GL_TRIANGLES);
	static VBOObject* createVBOObject(string name, std::vector<Vertex>* vertexData, GLenum primitiveType = GL_TRIANGLES,
		Material material = { 0.1f, { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.000001f });

	void setSpeed(float x, float y, float z) { 
		this->speed.x = x;
		this->speed.y = y;
		this->speed.z = z;
	}

	void sendMaterialDataToGPU(const Material material);

protected:
	GLuint createBufferObject(GLenum target, const void* bufferData, GLsizei bufferSize);
	GLuint createTextureObject(GLenum target, GLenum colorType, GLuint width, GLuint height, const void* bufferData);
	
	void renderObject(VBOObject * object);

	void renderVboObjects();

	bool shouldUpdateLight(glm::vec3& playerPosition, float maxDistance);
};

#endif

