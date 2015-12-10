#pragma once
#ifndef OGL_VERTEX_SHADER
#define OGL_VERTEX_SHADER

#include "OGLShaderCompiler.h"

#include <string>

class OGLVertexShader : public OGLShaderCompiler
{

public:
	OGLVertexShader( std::string source = "" );
	virtual ~OGLVertexShader();

	bool compile();

	static std::string stockSource();
};

#endif

