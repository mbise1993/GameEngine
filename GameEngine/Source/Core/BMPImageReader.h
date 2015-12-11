#pragma once
#ifndef BMP_IMAGE_READER
#define BMP_IMAGE_READER

#include "ImageFileReader.h"

class BMPImageReader : public ImageFileReader
{
private:
	unsigned char header[54];	// BMP file has 54 bytes header
	unsigned int dataPos;		// Position of the actual image data
	unsigned int imageSize;		// width * height * 3

public:
	BMPImageReader();
	~BMPImageReader();

	bool readImageFile(std::string filePath) override;
};

#endif