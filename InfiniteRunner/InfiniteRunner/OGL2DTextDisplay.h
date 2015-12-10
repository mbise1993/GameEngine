#pragma once
#ifndef OGL_2D_TEXT_DISPLAY
#define OGL_2D_TEXT_DISPLAY

#include "OGLGameObject.h"

#include <vector>
#include <string>

struct TextXY
{
	std::string text;
	float x, y;
};

struct GlyphPos
{
	float lx, ty;
	float rx, by;
};

class OGL2DTexture;

class OGL2DTextDisplay :
	public OGLGameObject
{
protected:
	float charWidth;
	float charHeight;
	std::vector<TextXY*> texts;
	OGL2DTexture* texture;
	GLuint vbo;

public:
	OGL2DTextDisplay( float charWidth, float charHeight );
	virtual ~OGL2DTextDisplay();

	void addText( const std::string& text, float x, float y );

	void setTexture( OGL2DTexture* texture ) { this->texture = texture; }

	void clear();

	void render();

protected:
	GlyphPos getTexPosition( char ch );

	void renderText( const std::string& text, float x, float y );

	void renderChar( char ch, float x, float y );
};

#endif

