#pragma once
#ifndef BASE_SHADER
#define BASE_SHADER

class Logger;

class BaseShader
{
protected:
	Logger* logger;

public:
	BaseShader();
	virtual ~BaseShader();

	void setLogger(Logger* logger) { this->logger = logger; }
};

#endif
