#pragma once
#ifndef TEXTURE
#define TEXTURE

#include <GL/glew.h>

struct Texture
{
	GLuint tbo;
	GLenum colorFormat;
	GLuint width;
	GLuint height;
	unsigned char* buffer;
};

#endif