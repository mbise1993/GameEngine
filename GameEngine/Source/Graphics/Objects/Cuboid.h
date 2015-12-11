#pragma once
#ifndef CUBOID
#define CUBOID

#include "OGL3DCompositeObject.h"

class FlatSurface;

class Cuboid : public OGL3DCompositeObject
{
protected:
	FlatSurface* bottom;
	FlatSurface* top;
	FlatSurface* front;
	FlatSurface* back;
	FlatSurface* left;
	FlatSurface* right;

	float height, width, depth;

public:
	Cuboid(const std::string& name, float height, float width, float depth, RGBA bottomColor, RGBA topColor,
		RGBA frontColor, RGBA backColor, RGBA leftColor, RGBA rightColor);
	virtual ~Cuboid();

	void setShaderProgram(GLuint shaderProgram);

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render();
	void render(OGLReferenceFrame& referenceFrame);
};

#endif