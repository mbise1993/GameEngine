#include "ConfigReader.h"

// 
// Accepts the name of the .config file to open and returns true or false based
// on whether or not the file was successfully opened with ifstream object.
//
bool ConfigReader::openFile( std::string fileName )
{
	_reader.open( fileName );
	return !_reader.fail();
}