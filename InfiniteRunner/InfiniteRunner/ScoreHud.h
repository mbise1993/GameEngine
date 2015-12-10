#pragma once
#ifndef SCORE_HUD
#define SCORE_HUD

#include "OGLHudObject.h"

class ScoreHud : public OGLHudObject
{
private:
	OGL2DTextDisplay* scoreDisplay;

public:
	ScoreHud( float width, float height, float posX, float posY, RGBA color, int initialScore );
	virtual ~ScoreHud();

	void setTextTexture( OGL2DTexture* texture );

	void setTextShaderProgram( GLuint shaderProgram );

	void render();

	void eventHandler( EventArgs* eventArgs );

private:
	void setScore( int score );
};

#endif