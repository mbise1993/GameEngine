#pragma once
#ifndef OGL_FRAGMENT_SHADER
#define OGL_FRAGMENT_SHADER

#include "OGLShaderCompiler.h"

#include <string>
using std::string;

#ifndef NULL
#define NULL 0
#endif

class OGLFragmentShader : public OGLShaderCompiler
{
public:
	OGLFragmentShader(string source = "");
	virtual ~OGLFragmentShader();

	bool compile();

	static string stockSource();
};

#endif

