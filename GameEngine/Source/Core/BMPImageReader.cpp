#include "BMPImageReader.h"
#include <fstream>

BMPImageReader::BMPImageReader()
{

}

BMPImageReader::~BMPImageReader()
{
	
}

bool BMPImageReader::readImageFile(std::string filePath)
{
	this->dataPos = 0;
	this->width = 0;
	this->height = 0;
	this->imageSize = 0;

	// Open file. If failure, return false
	FILE* file = std::fopen(filePath.c_str(), "rb");
	if (!file)
		return false;

	// Read file header. If length is not 54 bytes, return false
	if (std::fread(this->header, 1, 54, file) != 54)
		return false;

	// If header does not begin with 'BM', return false
	if ((this->header[0] != 'B') || (this->header[1] != 'M'))
		return false;

	// Read file metadata from header
	this->dataPos = *(int*)&(header[0x0A]);
	this->width = *(int*)&(header[0x12]);
	this->height = *(int*)&(header[0x16]);
	this->imageSize = *(int*)&(header[0x22]);

	// If info is missing, fill it in
	if (this->imageSize == 0)
		this->imageSize = this->width * this->height * 3;	// 3 = one byte for each RGB component
	
	if (this->dataPos == 0)
		this->dataPos = 54;		// Set data position to right after header

	// Allocate buffer for image data
	this->data = new unsigned char[this->imageSize];

	// Read image data into buffer and close file stream
	std::fread(this->data, 1, this->imageSize, file);
	std::fclose(file);

	return true;
}
