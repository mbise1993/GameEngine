#pragma once
#ifndef CUBOID
#define CUBOID

#include "BaseCuboid.h"

class Cuboid : public BaseCuboid
{
public:
	Cuboid( const std::string& name, float width = 1, float depth = 1, float height = 1, RGBA faceColor = { 1, 1, 1, 1 } );

	virtual ~Cuboid();

protected:
	void generate();
};

#endif

