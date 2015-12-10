#include "OGLHudObject.h"

#define CHAR_WIDTH_PERCENTAGE 0.1f
#define CHAR_HEIGHT_PERCENTAGE 0.33333f

OGLHudObject::OGLHudObject( const std::string& name, float width, float height, float posX, float posY, RGBA color ) :
	OGLGameObject( name )
{
	this->width = width;
	this->height = height;
	this->posX = posX;
	this->posY = posY;
	this->color = color;

	generate();
}

OGLHudObject::~OGLHudObject()
{
	delete this->headerDisplay;
	glDeleteVertexArrays( 1, &this->vao );
}

void OGLHudObject::setHeader( const std::string& text )
{
	// Calculate character dimensions
	float charWidth = this->width * CHAR_WIDTH_PERCENTAGE;
	float charHeight = this->height * CHAR_HEIGHT_PERCENTAGE;
	this->headerDisplay = new OGL2DTextDisplay( charWidth, charHeight );

	// Calculate where to position header so it is center aligned
	float headerWidth = text.size() * charWidth;
	float headerPosX = (this->width - headerWidth) / 2;
	headerPosX += this->posX;
	float headerPosY = this->posY - (charHeight / 5);
	this->headerDisplay->addText( text, headerPosX, headerPosY );
}

void OGLHudObject::render()
{
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBindVertexArray( this->vao );
	glUseProgram( this->shaderProgram );
	glBindBuffer( GL_ARRAY_BUFFER, this->vbo );

	// Enable positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	// Enable colors
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(sizeof( float ) * 12)
		);

	glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 36, (void*)this->data, GL_STATIC_DRAW );
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	glUseProgram( 0 );
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	this->headerDisplay->render();
}

void OGLHudObject::generate()
{
	this->data = new float[36];
	int i = 0;

	//
	// Left triangle positions
	//
	// Upper left corner is origin
	this->data[i++] = this->posX;
	this->data[i++] = this->posY;
	// Lower left corner
	this->data[i++] = this->posX;
	this->data[i++] = this->posY - this->height;
	// Upper right corner
	this->data[i++] = this->posX + this->width;
	this->data[i++] = this->posY;
	//
	// Right triangle positions
	//
	// Lower left corner
	this->data[i++] = this->posX;
	this->data[i++] = this->posY - this->height;
	// Lower right corner
	this->data[i++] = this->posX + this->width;
	this->data[i++] = this->posY - this->height;
	// Upper right corner
	this->data[i++] = this->posX + this->width;
	this->data[i++] = this->posY;

	//
	// Colors
	//
	while ( i < 36 )
	{
		this->data[i++] = this->color.red;
		this->data[i++] = this->color.green;
		this->data[i++] = this->color.blue;
		this->data[i++] = this->color.alpha;
	}

	glGenVertexArrays( 1, &this->vao );
	glGenBuffers( 1, &this->vbo );
}