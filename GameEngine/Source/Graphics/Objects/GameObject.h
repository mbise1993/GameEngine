#pragma once
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <string>
#include <RGBA.h>
#include <vector>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <Material.h>
using std::string;

class IBehavior;
class LightSource;

class GameObject
{
protected:
	string name;
	// Must be allocated on the heap
	std::vector<Vertex>* vertexData;
	// Must be allocated on the heap, the first element stores
	// the size
	short* indexData;
	bool visible;
	IBehavior* behavior;
	LightSource* lightSource;
	glm::vec3 previousPlayerPos;

public:
	GameObject(string name);
	virtual ~GameObject();

	void setVertexData(std::vector<Vertex>* vertexData) { this->vertexData = vertexData; }
	std::vector<Vertex>* getVertexData() { return this->vertexData; }

	void setIndexData(short* indexData) { this->indexData = indexData; }
	short* getIndexData() { return this->indexData; }

	virtual void setLightSource(LightSource* lightSource) { this->lightSource = lightSource; }
	LightSource* getLightSource() { return this->lightSource; }
	
	string getName() const { return this->name; }

	virtual void update(glm::vec3 playerPosition, float elapsedSeconds);

	virtual void render() = 0;

	void setVisibility(bool visible) { this->visible = visible; }
	bool isVisible() const { return this->visible; }

	void setBehavior(IBehavior* behavior);
};

#endif

