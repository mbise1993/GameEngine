#include "OGL3DGroupObject.h"
#include "IBehavior.h"

using std::string;

OGL3DGroupObject::OGL3DGroupObject( string groupName ) : OGL3DCompositeObject( groupName )
{}


OGL3DGroupObject::~OGL3DGroupObject()
{
	for each(auto object in this->objects)
	{
		delete object.second;
	}
	this->objects.clear();
}

void OGL3DGroupObject::addObject( OGL3DObject * object )
{
	this->objects[object->getName()] = object;
}

void OGL3DGroupObject::setShaderProgram( GLuint shaderProgram )
{
	for each(auto object in this->objects)
	{
		object.second->setShaderProgram( shaderProgram );
	}
}

void OGL3DGroupObject::update( float elapsedSeconds )
{
	if ( this->behavior != nullptr )
	{
		this->behavior->update( this, elapsedSeconds );
	}

	for each(auto object in this->objects)
	{
		object.second->update( elapsedSeconds );
	}
}

void OGL3DGroupObject::render( glm::mat4 orientation )
{
	for each(auto iterator in this->objects)
	{
		// Get the object
		OGL3DObject* object = iterator.second;

		// Calculate orientation and render
		object->render( orientation * object->referenceFrame.orientation );
	}
}

void OGL3DGroupObject::setTextures( OGL2DTexture * texture )
{
	for each(auto object in this->objects)
	{
		object.second->setTexture( texture );
	}
}
