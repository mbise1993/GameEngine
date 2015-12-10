#pragma once

#include "RGBA.h"

#include <string>
#include <vector>
#include <map>

struct OBJMaterial
{
	RGBA diffuse;
	RGBA ambient;
	RGBA specular;
	std::string texturePath;

	OBJMaterial();

};

class OBJMaterialLoader
{
private:
	std::map<std::string, OBJMaterial*> materialMap;
	bool isValid;

public:
	OBJMaterialLoader();
	~OBJMaterialLoader();

	void reset();

	bool hasData() { return this->isValid; }

	void loadFile( std::string filePath );

	/*
	Returns the material for the specified material.
	*/
	OBJMaterial * getMaterial( std::string materialName );
};
