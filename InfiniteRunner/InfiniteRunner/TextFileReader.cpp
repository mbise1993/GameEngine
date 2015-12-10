#include "TextFileReader.h"
#include "Logger.h"
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

TextFileReader::TextFileReader()
{
	this->contents = "";
	this->logger = nullptr;
}

TextFileReader::~TextFileReader( void )
{}

const string& TextFileReader::readContents( const string& filename )
{
	this->contents = "";
	string line;
	ifstream fin( filename.c_str() );
	if ( fin.is_open() )
	{
		while ( fin.good() )
		{
			getline( fin, line );
			this->contents += line + "\n";
		}
		fin.close();
	}
	else
	{
		if ( this->logger )
		{
			this->logger->log( "Cannot open file." );
		}
	}
	return contents;
}

