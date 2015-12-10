#pragma once
#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <map>
#include <string>

class Texture;
// Manages all the textures of the game engine
//
class TextureManager
{
protected:
	std::map<std::string, Texture*> textureMap;

public:
	TextureManager();
	virtual ~TextureManager();

	void addTexture( const std::string& textureName, Texture* texture );

	Texture* getTexture( const std::string& textureName );
};

#endif

