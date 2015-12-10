#pragma once
#ifndef ENGINE_CORE_STRING_HELPER_H
#define ENGINE_CORE_STRING_HELPER_H

#include "EngineTypes.h"

#include <vector>
#include <string>

/*
Houses commonly used string routines.
*/
class StringHelper
{
public:
	/*
	Splits a string into tokens separated by the specified delimiters.
	*/
	static void split( std::wstring &line, wchar_t delimiters[], std::vector<std::wstring>* tokens );
	static void split( std::string &line, char delimiters[], std::vector<std::string>* tokens );

	static std::string join( std::vector<std::string> *tokens, std::string delimiter, uint fromIndex = 0 );

	/*
	Returns true if the string is empty or only contains whitespace
	*/
	static bool isEmptyOrWhiteSpace( std::string &line );
};

#endif