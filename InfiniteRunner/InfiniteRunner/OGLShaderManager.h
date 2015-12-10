#pragma once
#ifndef OGL_SHADER_MANAGER
#define OGL_SHADER_MANAGER

#include <string>
#include <map>

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "OGLShaderProgram.h"
#include "ShaderManager.h"
#include "Uniform.h"
#include "MathUtil.h"

class OGLShaderCompiler;

struct ShadingUnit
{
	OGLShaderProgram * shaderProgram;
	std::string vertexShaderName;
	std::string fragmentShaderName;
};

class OGLShaderManager : public ShaderManager
{
protected:
	std::map<std::string, OGLShaderCompiler *> vertexShaders;
	std::map<std::string, OGLShaderCompiler *> fragmentShaders;
	std::map<std::string, ShadingUnit> shadingUnits;

public:
	OGLShaderManager();

	~OGLShaderManager();

	void addVertexShader( std::string name, OGLShaderCompiler* vertexShader );

	void addFragmentShader( std::string name, OGLShaderCompiler* fragmentShader );

	void addShaderProgram(
		std::string name,
		OGLShaderProgram* shaderProgram,
		std::string vertexShaderName,
		std::string fragmentShaderName );

	void addUniform( std::string uniformName, UniformDataType type, const std::string& shaderName );

	Uniform* getUniform( const std::string& uniformName, const std::string& shaderName );

	bool createShader( const std::string& shaderName );

	bool createShaders();

	GLuint getShaderHandle( std::string shaderName );

	BaseShader* getShader( const std::string& shaderName );

private:
	bool compileVertexShader( std::string vertexShaderName );
	bool compileFragmentShader( std::string fragmentShaderName );
	bool linkShader( std::string shaderName, OGLShaderCompiler * vertexShader, OGLShaderCompiler * fragmentShader );
};

#endif

