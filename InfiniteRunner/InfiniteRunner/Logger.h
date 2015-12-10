#pragma once
#ifndef LOGGER
#define LOGGER

#include <string>

class Logger
{
public:
	Logger();
	virtual ~Logger();

	virtual void log( std::string text ) = 0;
	virtual void log( std::wstring text ) = 0;
	virtual void debug( std::string text ) = 0;
	virtual void debug( std::wstring text ) = 0;
};

#endif

