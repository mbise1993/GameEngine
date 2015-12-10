#pragma once
#ifndef OGL_BASE_SHADER
#define OGL_BASE_SHADER

#include "BaseShader.h"
#include "Uniform.h"

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <string>
#include <map>

class OGLBaseShader : public BaseShader
{
protected:
	GLuint handle;

public:
	std::map<std::string, Uniform*> uniforms;

public:
	OGLBaseShader();
	virtual ~OGLBaseShader();

	GLuint getHandle() const { return this->handle; }

	void addUniform( std::string uniformName, UniformDataType type );

	void setUniformData( const std::string& uniformName, void *data );

	void  sendData( const std::string& uniformName );

	Uniform* getUniform( const std::string& uniformName );

protected:
	void showInfoLog( GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog );

	void sendMatrix4( GLuint location, void *matrix );

	void sendVector3( GLuint location, void *vector );

	void sendVector4( GLuint location, void *vector );

	void sendVector3Array( GLuint location, void *vectorArray, unsigned int count );

	void sendInteger( GLuint location, void *value );

	void sendFloat( GLuint location, void *value );

	void sendSampler2D( GLuint location, void *value );

	void sendFloatArray( GLuint location, void *values, unsigned int count );


};

#endif

