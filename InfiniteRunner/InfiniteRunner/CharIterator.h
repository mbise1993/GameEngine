#pragma once
#ifndef CharIterator_h
#define CharIterator_h

#include "EngineTypes.h"

#include <string>
#include <vector>

class CharIterator
{
private:
	char* source;
	size_t length;

public:

	CharIterator();

	CharIterator( CharIterator& source );

	CharIterator( char* source, size_t length );

	virtual ~CharIterator();

	/*
	Get the source of the iterator
	*/
	inline char* getSource() { return this->source; }

	/*
	Get the length of the iterator
	*/
	inline size_t getLength() { return this->length; }

	/*
	Split the iterator into fields
	Note: this is destructive, the delimiter will be replaced will nulls
	  This greatly simplifies using the delimited fields, as they will conform
	  to a standard null terminated c_string.
	*/
	void split( char delimiter, std::vector<CharIterator> *tokens );

	/*
	Convert to string
	*/
	inline std::string toString() { return std::string( this->source, length ); }

	/*
	Convert to float
	*/
	inline float toFloat() { return (float)atof( this->source ); }

	/*
	Convert to integer
	*/
	inline int toInt() { return atoi( this->source ); }

	/*
	Convert to unsigned integer
	*/
	inline uint toUInt() { return (uint)atol( this->source ); }

	/*
	Convert to long
	*/
	inline long toLong() { return atol( this->source ); }

	/*
	Compares the iterator to a char*
	*/
	inline bool equals( const char* other ) { return (strcmp( this->source, other ) == 0); }
	inline bool operator== ( const char* rhs ) { return (strcmp( this->source, rhs ) == 0); }

	/*
	Compares the iterator to another iterator
	*/
	bool equals( const CharIterator& other );
	inline bool operator== ( const CharIterator &rhs ) { return this->equals( rhs ); }

	/*
	Index operator.
	*/
	inline char& operator[] ( const uint nIndex ) { return this->source[nIndex]; }

	// Read in a file and create an iterator
	static CharIterator FromFile( std::string filePath );
};
#endif
