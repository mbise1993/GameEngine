#pragma once
#ifndef OBJECT_FILE_READER
#define OBJECT_FILE_READER

#include <string>
#include <vector>
#include <Objects/OGLObject.h>

class ObjectFileReader
{
protected:
	std::vector<float> vertexData;
	std::vector<short> indexData;

public:
	ObjectFileReader() { }
	~ObjectFileReader() { }

	virtual bool readFile(std::string filePath) = 0;
	virtual OGLObject* createObject(std::string name) = 0;
};

#endif