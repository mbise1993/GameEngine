#pragma once
#ifndef TEXT_FILE_READER
#define TEXT_FILE_READER

#include <string>
using std::string;
#include <vector>
using std::vector;

class Logger;

/**
  Has the responsibility of reading data from a text file
*/
class TextFileReader
{
private:
	// The file's content as a string
	string contents;
	// The logger
	Logger * logger;

public:
	// Constructor
	TextFileReader();
	
	// Destructor
	~TextFileReader(void);

	// Reads the contents of the file and returns it as a single string
	// Params:
	//    filename - the name of the from which data will be read
	// Returns:
	//    the contents of the file
	const string& readContents(const string& filename);

	// Getter
	string getContexts() const { return contents; }

	// Sets the logger
	void setLogger(Logger * logger) { this->logger = logger;  }
};

#endif

