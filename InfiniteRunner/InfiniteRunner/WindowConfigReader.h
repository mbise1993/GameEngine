#pragma once
#ifndef WINDOW_CONFIG_READER_H
#define WINDOW_CONFIG_READER_H

#include "ConfigReader.h"
#include "WindowInfo.h"

//
// Class derived from ConfigReader which specifically reads window configuration file
//
class WindowConfigReader : public ConfigReader
{
private:
	bool isFileOpen;

public:
	WindowConfigReader( std::string filePath );
	~WindowConfigReader() {}
	bool isOpen();
	void readConfigFile( WindowInfo& windowInfo );
};

#endif