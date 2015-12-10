#pragma once
#ifndef OGL_FRAGMENT_SHADER
#define OGL_FRAGMENT_SHADER

#include "OGLShaderCompiler.h"

#include <string>

class OGLFragmentShader : public OGLShaderCompiler
{
public:
	OGLFragmentShader( std::string source = "" );
	virtual ~OGLFragmentShader();

	bool compile();

	static std::string stockSource();
};

#endif

