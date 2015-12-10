#include "Skybox.h"
#include "OGLTexturedFlatSurface.h"
#include "OGLElementArrayBuffer.h"

Skybox::Skybox( const std::string& name, float size ) : OGL3DCompositeObject( name )
{
	this->size = size;

	this->bottom = new OGLTexturedFlatSurface( "Bottom", this->size, this->size, 1, 1 );
	this->top = new OGLTexturedFlatSurface( "Top", this->size, this->size, 1, 1 );
	this->front = new OGLTexturedFlatSurface( "Front", this->size, this->size, 1, 1 );
	this->back = new OGLTexturedFlatSurface( "Back", this->size, this->size, 1, 1 );
	this->left = new OGLTexturedFlatSurface( "Left", this->size, this->size, 1, 1 );
	this->right = new OGLTexturedFlatSurface( "Right", this->size, this->size, 1, 1 );
}


Skybox::~Skybox()
{
	delete this->bottom;
	delete this->top;
	delete this->front;
	delete this->back;
	delete this->left;
	delete this->right;
}

void Skybox::setTexture( SkyboxSide side, Texture* texture )
{
	switch ( side )
	{
	case SKYBOX_BOTTOM:
		this->bottom->setTexture( texture );
		break;
	case SKYBOX_TOP:
		this->top->setTexture( texture );
		break;
	case SKYBOX_FRONT:
		this->front->setTexture( texture );
		break;
	case SKYBOX_BACK:
		this->back->setTexture( texture );
		break;
	case SKYBOX_LEFT:
		this->left->setTexture( texture );
		break;
	case SKYBOX_RIGHT:
		this->right->setTexture( texture );
		break;
	}
}

void Skybox::setShaderProgram( GLuint shaderProgram )
{
	this->shaderProgram = shaderProgram;
	this->bottom->setShaderProgram( shaderProgram );
	this->top->setShaderProgram( shaderProgram );
	this->front->setShaderProgram( shaderProgram );
	this->back->setShaderProgram( shaderProgram );
	this->left->setShaderProgram( shaderProgram );
	this->right->setShaderProgram( shaderProgram );
}

void Skybox::update( float elapsedSeconds )
{
	OGL3DCompositeObject::update( elapsedSeconds );
}

//
// Render "inverted cube" in hierarchical fashion with bottom as root:
// -> Bottom
//     -> Top
//     -> Front
//         -> Back
//     -> Left
//         -> Right
//
void Skybox::render()
{
	glDepthMask( GL_FALSE );

	// Bottom
	this->bottom->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame( this->bottom->referenceFrame );
	this->bottom->render();
	this->frameStack.push();
	{
		// Top
		this->frameStack.translate( 0, this->size, 0 );
		this->frameStack.rotateX( 180 );
		this->top->render( this->frameStack.top() );
	}
	this->frameStack.pop();

	this->frameStack.push();
	{
		// Front
		this->frameStack.translate( 0, 0, this->size / 2 );
		this->frameStack.translate( 0, this->size / 2, 0 );
		this->frameStack.rotateX( -90 );
		this->front->render( this->frameStack.top() );
		this->frameStack.push();
		{
			// Back
			this->frameStack.translate( 0, this->size, 0 );
			this->frameStack.rotateX( 180 );
			this->back->render( this->frameStack.top() );
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();

	this->frameStack.push();
	{
		// Left
		this->frameStack.translate( -this->size / 2, 0, 0 );
		this->frameStack.translate( 0, this->size / 2, 0 );
		this->frameStack.rotateZ( -90 );
		this->left->render( this->frameStack.top() );
		this->frameStack.push();
		{
			// Right
			this->frameStack.translate( 0, this->size, 0 );
			this->frameStack.rotateZ( 180 );
			this->right->render( this->frameStack.top() );
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();

	glDepthMask( GL_TRUE );
}
