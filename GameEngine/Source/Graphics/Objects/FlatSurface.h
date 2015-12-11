#pragma once
#ifndef FLAT_SURFACE
#define FLAT_SURFACE

#include "OGL3DObject.h"

class FlatSurface : public OGL3DObject
{
protected:
	float width, depth;
	RGBA color;

public:
	FlatSurface(const std::string& name, float width, float depth, RGBA color);
	virtual ~FlatSurface();

protected:
	void generate();
};

#endif