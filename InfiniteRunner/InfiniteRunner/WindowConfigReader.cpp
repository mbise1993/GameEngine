#include "WindowConfigReader.h"
//
// Constructor calls openFile method of base class to open window.config file
//
WindowConfigReader::WindowConfigReader( std::string filePath )
{
	this->isFileOpen = this->openFile( filePath );
}

//
// Returns boolean value stating whether or not the file was opened successfully
//
bool WindowConfigReader::isOpen()
{
	return this->isFileOpen;
}

//
// Accepts reference to WindowInfo object which is filled using the readAttribute
// method of the base class.
//
void WindowConfigReader::readConfigFile( WindowInfo& windowInfo )
{
	// Get window title
	windowInfo.title = this->readAttribute<std::wstring>( "title" );
	// Get x and y coordinates for window start position
	windowInfo.startX = this->readAttribute<int>( "start-position-x" );
	windowInfo.startY = this->readAttribute<int>( "start-position-y" );
	// Get width and height
	windowInfo.width = this->readAttribute<int>( "width" );
	windowInfo.height = this->readAttribute<int>( "height" );
	// Get RGBA values for window background color
	windowInfo.backgroundColor.red = this->readAttribute<int>( "background-color-red" ) / 255.0f;
	windowInfo.backgroundColor.green = this->readAttribute<int>( "background-color-green" ) / 255.0f;
	windowInfo.backgroundColor.blue = this->readAttribute<int>( "background-color-blue" ) / 255.0f;
	windowInfo.backgroundColor.alpha = this->readAttribute<int>( "background-color-alpha" ) / 255.0f;
}