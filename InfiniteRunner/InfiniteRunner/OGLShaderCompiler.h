#pragma once
#ifndef OGL_SHADER_COMPILER
#define OGL_SHADER_COMPILER

#include "OGLBaseShader.h"

#include <string>

class OGLShaderCompiler :
	public OGLBaseShader
{
protected:
	std::string sourceCode;

public:
	OGLShaderCompiler( std::string sourceCode = "" );
	virtual ~OGLShaderCompiler();

	virtual bool compile() = 0;

	void setSource( std::string source ) { this->sourceCode = source; }

protected:
	void compile( GLenum type );
};

#endif

