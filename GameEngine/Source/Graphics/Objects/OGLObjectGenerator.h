#pragma once
#ifndef OGL_OBJECT_GENERATOR
#define OGL_OBJECT_GENERATOR

#include <string>

struct RGBA;
class OGLObject;

class OGLObjectGenerator
{
public:
	static OGLObject* generateFlatSurface(std::string name, float width, float depth, RGBA color);

	static OGLObject* generateRoom(std::string name, float width, float height, float depth, RGBA floorColor, 
		RGBA ceilingColor, RGBA frontWallColor, RGBA backWallColor, RGBA leftWallColor, RGBA rightWallColor);

	static OGLObject* generateCuboid(std::string name, float width, float height, float depth, RGBA color);
};

#endif