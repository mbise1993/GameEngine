#pragma once
#ifndef HEALTH_HUD
#define HEALTH_HUD

#include "OGLHudObject.h"

class HealthHud : public OGLHudObject
{
private:
	OGL2DTextDisplay* healthDisplay;

public:
	HealthHud( float width, float height, float posX, float posY, RGBA color, int initialHealth );
	virtual ~HealthHud();

	void setTextTexture( OGL2DTexture* texture );

	void setTextShaderProgram( GLuint shaderProgram );

	void render();

	void eventHandler( EventArgs* eventArgs );

private:
	void setHealth( int health );
};

#endif
