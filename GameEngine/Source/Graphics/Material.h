#ifndef MATERIAL
#define MATERIAL

#include "RGBA.h"

struct Material {
	float ambientIntensity; // Light emitted by this object
	RGBA diffuseColor;
	RGBA specularColor;
	float specularExp;
};

#endif