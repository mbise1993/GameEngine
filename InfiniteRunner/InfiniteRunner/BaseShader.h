#pragma once
#ifndef BASE_SHADER
#define BASE_SHADER

#include <string>

class Logger;

class BaseShader
{
protected:
	Logger* logger;

public:
	BaseShader();
	virtual ~BaseShader();

	void setLogger( Logger* logger ) { this->logger = logger; }

	virtual void sendData( const std::string& key ) = 0;
};

#endif
