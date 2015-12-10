#pragma once
#ifndef OGL_3D_OBJECT
#define OGL_3D_OBJECT

#include "OGLObject.h"

#include <string>

class OGL3DObject :
	public OGLObject
{
protected:
	std::string uniformMatrixName;

public:
	OGL3DObject( const std::string& name, const std::string& uniformMatrixName = "localToWorldMatrix" );

	virtual ~OGL3DObject();

	void setUniformMatrixName( const std::string& name )
	{
		this->uniformMatrixName = name;
	}

	void render();

	virtual void render( OGLReferenceFrame& frame );

	virtual void render( glm::mat4 orientation );

protected:
	void sendMatrixToGPU( const glm::mat4& mat );
};

#endif

