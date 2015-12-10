#include "CharIterator.h"

#include <fstream>

using namespace std;

CharIterator::CharIterator()
{
	this->source = nullptr;
	this->length = 0;
}

CharIterator::CharIterator( CharIterator & source )
{
	this->source = source.source;
	this->length = source.length;
}

CharIterator::CharIterator( char * source, size_t length )
{
	this->source = source;
	this->length = length;
}

CharIterator::~CharIterator()
{}

bool CharIterator::equals( const CharIterator& other )
{
	if ( this->length != other.length ) return false;
	return (strcmp( this->source, other.source ) == 0);
}

void CharIterator::split( char delimiter, std::vector<CharIterator>* tokens )
{
	size_t curPos = 0, startPos = 0, endPos = this->length;

	for ( curPos = 0; curPos < endPos; ++curPos )
	{
		if ( this->source[curPos] == delimiter )
		{
			this->source[curPos] = 0;
			tokens->push_back( CharIterator( (this->source + startPos), (curPos - startPos) ) );
			startPos = curPos + 1;
		}
	}
	if ( startPos != endPos )
	{
		tokens->push_back( CharIterator( (this->source + startPos), (endPos - startPos) ) );
	}
}

CharIterator CharIterator::FromFile( std::string filePath )
{
	// Open the file
	ifstream stream{ filePath, ios::in };

	// Was the file opened?
	if ( stream.fail() )
	{
		return CharIterator{ nullptr, 0 };
	}

	// Move to the end of the file
	stream.seekg( 0, ios::end );

	// Get the file size
	size_t fileSize = (size_t)stream.tellg();

	// Create the buffer
	char* buffer = new char[1 + fileSize];

	// Null terminate the data.
	buffer[fileSize] = 0;

	// Move back to the begining of the file
	stream.seekg( 0, ios::beg );

	// Read the file
	stream.read( buffer, fileSize );

	// Close the file
	stream.close();

	// Return the iterator
	return CharIterator( buffer, fileSize );
}
