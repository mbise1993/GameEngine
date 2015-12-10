#pragma once
#ifndef OGL_HUD_OBJECT
#define OGL_HUD_OBJECT

#include "Subscriber.h"
#include "OGL2DTextDisplay.h"
#include <string>

class OGLHudObject : public OGLGameObject, public Subscriber
{
protected:
	float width, height;
	RGBA color;
	float posX, posY;
	OGL2DTextDisplay* headerDisplay;
	float* data;
	GLuint vbo;

public:
	OGLHudObject( const std::string& name, float width, float height, float posX, float posY, RGBA color );
	virtual ~OGLHudObject();

	virtual void setTextTexture( OGL2DTexture* texture ) { this->headerDisplay->setTexture( texture ); }

	virtual void setTextShaderProgram( GLuint shaderProgram ) { this->headerDisplay->setShaderProgram( shaderProgram ); }

	virtual void render();

	virtual void eventHandler( EventArgs* eventArgs ) = 0;

protected:
	void setHeader( const std::string& text );

private:
	void generate();
};

#endif