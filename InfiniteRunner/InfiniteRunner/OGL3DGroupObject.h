#pragma once
#include "OGL3DCompositeObject.h"
#include "OGL2DTexture.h"

#include <map>

class OGL3DGroupObject :
	public OGL3DCompositeObject
{
private:
	std::map<std::string, OGL3DObject*> objects;

public:
	OGL3DGroupObject( std::string groupName );
	~OGL3DGroupObject();

	void addObject( OGL3DObject* object );

	void setShaderProgram( GLuint shaderProgram );

	void update( float elapsedSeconds );

	void render( glm::mat4 orientation );

	void setTextures( OGL2DTexture* texture );
};

