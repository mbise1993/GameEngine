#pragma once
#include "OGLHudObject.h"
class GameOverHUD :
	public OGLHudObject
{
protected:
	OGL2DTextDisplay* scoreDisplay;
	OGL2DTextDisplay* scoreHeader;
public:
	GameOverHUD(float width, float height, float posX, float posY, RGBA color);
	~GameOverHUD();
	void setTextTexture(OGL2DTexture * texture);
	void setTextShaderProgram(GLuint shaderProgram);
	void setScore(int score);
	void render();
	void eventHandler(EventArgs * eventArgs);
};

