#pragma once
#ifndef SHADER_MANAGER
#define SHADER_MANAGER

#include "BaseObject.h"

#include <string>

class Logger;
class BaseShader;

class ShaderManager : public BaseObject
{
public:
	ShaderManager();

	virtual ~ShaderManager();

	virtual bool createShader( const std::string& shaderName ) = 0;

	virtual BaseShader* getShader( const std::string& shaderName ) = 0;
};

#endif

