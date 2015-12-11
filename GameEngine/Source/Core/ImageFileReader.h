#pragma once
#ifndef IMAGE_FILE_READER
#define IMAGE_FILE_READER

#include <string>

//
// Base class for creating image file readers
//
class ImageFileReader
{
protected:
	unsigned char* data;		// Raw image data
	unsigned int width;			// Width of the image
	unsigned int height;		// Height of the image

public:
	ImageFileReader();
	virtual ~ImageFileReader();

	virtual bool readImageFile(std::string filePath) = 0;
	virtual unsigned char* getData() { return this->data; }
	virtual unsigned int getWidth() { return this->width; }
	virtual unsigned int getHeight() { return this->height; }
};

#endif