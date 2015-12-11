#pragma once
#ifndef TEXTURE_READER
#define TEXTURE_READER

#include "Texture.h"
#include <string>

class ImageFileReader;

class TextureReader
{
private:
	ImageFileReader* imageReader;

public:
	TextureReader();
	~TextureReader();

	bool readTexture(std::string filePath);
	Texture* createTexture();
};

#endif