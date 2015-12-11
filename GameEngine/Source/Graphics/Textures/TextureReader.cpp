#include "TextureReader.h"
#include "BMPImageReader.h"

TextureReader::TextureReader()
{
	this->imageReader = NULL;
}

TextureReader::~TextureReader()
{
	if(this->imageReader)
		delete this->imageReader;
}

bool TextureReader::readTexture(std::string filePath)
{
	// Check for valid file extensions. If none found, return false
	if (filePath.substr(filePath.find_last_of('.') + 1) == "bmp")
		this->imageReader = new BMPImageReader();
	else
		return false;

	// If unable to read file, return false
	if (!this->imageReader->readImageFile(filePath))
		return false;

	return true;
}

Texture* TextureReader::createTexture()
{
	Texture texture;
	texture.buffer = this->imageReader->getData();
	texture.width = this->imageReader->getWidth();
	texture.height = this->imageReader->getHeight();

	return &texture;
}
