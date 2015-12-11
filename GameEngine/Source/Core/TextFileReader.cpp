#include "TextFileReader.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

//
// Initialize fields to default values
//
TextFileReader::TextFileReader()
{
	this->contents = "";
	this->logger = NULL;
}

TextFileReader::~TextFileReader(void)
{
}

//
// Read entire contents from specified file
//
const string& TextFileReader::readContents(const string& filename)
{
	this->contents = "";
	string line;
	ifstream fin(filename.c_str());
	if(fin.is_open())
	{
		while(fin.good())
		{
			getline(fin, line);
			this->contents += line + "\n";
		}

		fin.close();
	}
	else
	{
		if (this->logger) 
			this->logger->log("Cannot open file.");
	}

	return contents;
}

