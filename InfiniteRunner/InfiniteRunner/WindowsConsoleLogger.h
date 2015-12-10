#pragma once
#ifndef WINDOWS_CONSOLE_LOGGER
#define WINDOWS_CONSOLE_LOGGER

#include <Windows.h>
#include <string>

#include "Logger.h"

#define _LOGGING_

class WindowsConsoleLogger :
	public Logger
{
private:
	static WindowsConsoleLogger* INSTANCE;
	HANDLE stdOut;
	wchar_t *buffer;
	int size;

public:
	static const int MAX_CONSOLE_LINES = 500;
	static const int BUFFER_SIZE = 256;

public:
	WindowsConsoleLogger( void );
	~WindowsConsoleLogger( void );


	static WindowsConsoleLogger* getInstance() { return WindowsConsoleLogger::INSTANCE; }

	void log( std::string text );
	void log( std::wstring text );
	void debug( std::string text );
	void debug( std::wstring text );

private:
	void create();
	void write();
};

typedef WindowsConsoleLogger WCLogger;

#endif

