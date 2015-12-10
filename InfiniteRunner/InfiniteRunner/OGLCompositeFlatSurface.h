#pragma once
#ifndef OGL_COMPOSITE_FLAT_SURFACE
#define OGL_COMPOSITE_FLAT_SURFACE

#include "OGL3DCompositeObject.h"

#include <string>
#include <vector>

class OGLTexturedFlatSurface;

class OGLCompositeFlatSurface : public OGL3DCompositeObject
{
public:
	std::vector<OGLTexturedFlatSurface*> surfaces;

public:
	OGLCompositeFlatSurface( const std::string& name );

	virtual ~OGLCompositeFlatSurface();

	void setShaderProgram( GLuint shaderProgram );

	void update( float elapsedSeconds );

	void render();
};

#endif

