#pragma once
#ifndef CORRIDER
#define CORRIDER

#include <map>
#include "OGL3DCompositeObject.h"

class FlatSurface;
class Cuboid;

class Corrider : public OGL3DCompositeObject
{
private:
	FlatSurface* floor;
	FlatSurface* ceiling;
	Cuboid* leftWall;
	Cuboid* rightWall;

	float doorwayHeight, doorwayWidth, corriderLength;

public:
	Corrider(const std::string& name, float doorwayHeight, float doorwayWidth, float corriderLength, 
		RGBA floorColor, RGBA ceilingColor, RGBA leftWallColor, RGBA rightWallColor);
	virtual ~Corrider();

	void setShaderProgram(GLuint shaderProgram);

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render();
};

#endif