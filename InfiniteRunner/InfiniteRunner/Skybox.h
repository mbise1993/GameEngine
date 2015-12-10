#pragma once
#ifndef SKYBOX
#define SKYBOX

#include "OGL3DCompositeObject.h"

class OGLTexturedFlatSurface;

enum SkyboxSide
{
	SKYBOX_BOTTOM,
	SKYBOX_TOP,
	SKYBOX_FRONT,
	SKYBOX_BACK,
	SKYBOX_LEFT,
	SKYBOX_RIGHT,
};

class Skybox : public OGL3DCompositeObject
{
protected:
	OGLTexturedFlatSurface* bottom;
	OGLTexturedFlatSurface* top;
	OGLTexturedFlatSurface* front;
	OGLTexturedFlatSurface* back;
	OGLTexturedFlatSurface* left;
	OGLTexturedFlatSurface* right;

	float size;

public:
	Skybox( const std::string& name, float size );
	virtual ~Skybox();

	void setTexture( SkyboxSide side, Texture* texture );

	void setShaderProgram( GLuint shaderProgram );

	void update( float elapsedSeconds );

	void render();
};

#endif