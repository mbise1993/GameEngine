#include "StringHelper.h"

using namespace std;

void StringHelper::split( wstring &line, wchar_t delimiters[], vector<wstring>* tokens )
{
	wchar_t * token;
	wchar_t * tLine = const_cast<wchar_t *>(line.c_str());
	wchar_t *context = NULL;

	// Get the first token
	token = wcstok_s( tLine, delimiters, &context );

	while ( token != NULL )
	{
		// Add to vector
		tokens->push_back( wstring( token ) );

		// Get the next token
		token = wcstok_s( NULL, delimiters, &context );
	}
}

void StringHelper::split( string &line, char delimiters[], vector<string>* tokens )
{
	char *token;
	char *tLine = const_cast<char*>(line.c_str());
	char *context = nullptr;

	// Get the first token
	token = strtok_s( tLine, delimiters, &context );

	while ( token != nullptr )
	{
		// Add to vector
		tokens->push_back( string( token ) );

		// Get the next token
		token = strtok_s( nullptr, delimiters, &context );
	}
}

string StringHelper::join( vector<string>* tokens, string delimiter, uint fromIndex )
{
	string results = "";

	for ( uint i = fromIndex; i < tokens->size(); ++i )
	{
		if ( i > fromIndex )
		{
			results += " " + (*tokens)[i];
		}
		else
		{
			results += (*tokens)[i];
		}
	}

	return results;
}

bool StringHelper::isEmptyOrWhiteSpace( string & line )
{
	// Empty check
	if ( line.empty() )
	{
		return true;
	}

	// Check whitespace, spaces and tabs
	return (line.find_first_not_of( " \t" ) == string::npos);
}
