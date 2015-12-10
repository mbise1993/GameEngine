#include "HealthHud.h"
#include "HealthChangedEventArgs.h"
#include <boost\lexical_cast.hpp>

#define CHAR_WIDTH_PERCENTAGE 0.14285f
#define CHAR_HEIGHT_PERCENTAGE 0.66666f

HealthHud::HealthHud( float width, float height, float posX, float posY, RGBA color, int initialHealth ) :
	OGLHudObject( "HealthHud", width, height, posX, posY, color )
{
	OGLHudObject::setHeader( "Health" );

	// Calculate character dimensions
	float charWidth = width * CHAR_WIDTH_PERCENTAGE;
	float charHeight = height * CHAR_HEIGHT_PERCENTAGE;
	this->healthDisplay = new OGL2DTextDisplay( charWidth, charHeight );

	setHealth( initialHealth );
}

HealthHud::~HealthHud()
{
	delete this->healthDisplay;
}

void HealthHud::setTextTexture( OGL2DTexture* texture )
{
	this->healthDisplay->setTexture( texture );
	OGLHudObject::setTextTexture( texture );
}

void HealthHud::setTextShaderProgram( GLuint shaderProgram )
{
	this->healthDisplay->setShaderProgram( shaderProgram );
	OGLHudObject::setTextShaderProgram( shaderProgram );
}

void HealthHud::setHealth( int health )
{
	this->healthDisplay->clear();

	std::string healthStr = boost::lexical_cast<std::string>(health);

	// Calculate where to position health so it is center aligned
	float healthWidth = healthStr.size() * (this->width * CHAR_WIDTH_PERCENTAGE);
	float healthPosX = (this->width - healthWidth) / 2;
	healthPosX += this->posX;
	float healthPosY = (this->posY - this->height) + (this->height * CHAR_HEIGHT_PERCENTAGE);
	this->healthDisplay->addText( healthStr, healthPosX, healthPosY );
}

void HealthHud::render()
{
	OGLHudObject::render();
	this->healthDisplay->render();
}

void HealthHud::eventHandler( EventArgs* eventArgs )
{
	if ( eventArgs->getEventType() != HEALTH_CHANGED_EVENT ) return;

	HealthChangedEventArgs* healthChangedEventArgs = dynamic_cast<HealthChangedEventArgs*>(eventArgs);
	if ( healthChangedEventArgs != nullptr )
	{
		setHealth( healthChangedEventArgs->getHealthDelta() );
	}
}