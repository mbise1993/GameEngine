#include "WindowsConsoleLogger.h"

//
// Initialize fields to default values
//
WindowsConsoleLogger::WindowsConsoleLogger(void)
{
	this->stdOut = NULL;
	this->buffer = NULL;
	this->size = 0;
	create();
}

//
// Delete buffer
//
WindowsConsoleLogger::~WindowsConsoleLogger(void)
{
	if (this->buffer) 
		delete[] this->buffer;
}

//
// Create console and set standard output handle
//
void WindowsConsoleLogger::create()
{
	this->stdOut = NULL;

	if (AllocConsole() == 0) return;

	this->stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->stdOut == INVALID_HANDLE_VALUE || this->stdOut == NULL) 
	{
		this->stdOut = NULL;
		return;
	}
}

//
// Write to console
//
void WindowsConsoleLogger::write()
{
	if (this->stdOut != NULL) 
	{
		DWORD check;
		WriteConsoleW(this->stdOut, (void*)this->buffer, this->size, &check, NULL);
	}
}

//
// Write a string to console
//
void WindowsConsoleLogger::log(string text)
{
	text += "\n";
	this->size = text.length();
	if (this->size > BUFFER_SIZE) 
		this->size = BUFFER_SIZE;
	
	if (this->buffer) 
		delete[] this->buffer;
	
	this->buffer = new wchar_t[this->size];

	int i = 0;
	for (i = 0; i < this->size; i++) 
		this->buffer[i] = text[i];
	
	write();
}

//
// Write a wstring to console
//
void WindowsConsoleLogger::log(wstring text)
{
	text += L"\n";
	this->size = text.length();
	if (this->size > BUFFER_SIZE) 
		this->size = BUFFER_SIZE;
	
	if (this->buffer) 
		delete[] this->buffer;
	
	this->buffer = new wchar_t[this->size];

	int i = 0;
	for (i = 0; i < this->size; i++) 
		this->buffer[i] = text[i];
	
	write();
}

//
// Write a debugging string to console
//
void WindowsConsoleLogger::debug(string text)
{
	log("Debug: '" + text + "'");
}

//
// Write a debugging wstring to console
//
void WindowsConsoleLogger::debug(wstring text)
{
	log(L"Debug: '" + text + L"'");
}


