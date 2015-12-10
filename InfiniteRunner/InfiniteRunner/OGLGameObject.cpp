#include "OGLGameObject.h"

OGLGameObject::OGLGameObject( const std::string& name ) : GameObject( name )
{
	this->lineBox = nullptr;
}

OGLGameObject::~OGLGameObject()
{}
