#pragma once
#ifndef BMP_TEXTURE_LOADER
#define BMP_TEXTURE_LOADER

#include "ITextureLoader.h"

#include <string>

class Texture;

class BMPTextureLoader :
	public ITextureLoader
{
protected:
	std::string filepath;

private:
	std::string status;

public:
	BMPTextureLoader();
	virtual ~BMPTextureLoader();

	void setFilePath( const std::string& filepath ) { this->filepath = filepath; }

	const std::string& getFileStatus() { return this->status; }

	Texture* load();
};

#endif

