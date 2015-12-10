#include "ScoreHud.h"
#include "ScoreChangedEventArgs.h"
#include <boost\lexical_cast.hpp>

#define CHAR_WIDTH_PERCENTAGE 0.14285f
#define CHAR_HEIGHT_PERCENTAGE 0.66666f

ScoreHud::ScoreHud( float width, float height, float posX, float posY, RGBA color, int initialScore ) :
	OGLHudObject( "ScoreHud", width, height, posX, posY, color )
{
	OGLHudObject::setHeader( "Score" );

	// Calculate character dimensions
	float charWidth = width * CHAR_WIDTH_PERCENTAGE;
	float charHeight = height * CHAR_HEIGHT_PERCENTAGE;
	this->scoreDisplay = new OGL2DTextDisplay( charWidth, charHeight );

	setScore( initialScore );
}

ScoreHud::~ScoreHud()
{
	delete this->scoreDisplay;
}

void ScoreHud::setTextTexture( OGL2DTexture* texture )
{
	this->scoreDisplay->setTexture( texture );
	OGLHudObject::setTextTexture( texture );
}

void ScoreHud::setTextShaderProgram( GLuint shaderProgram )
{
	this->scoreDisplay->setShaderProgram( shaderProgram );
	OGLHudObject::setTextShaderProgram( shaderProgram );
}

void ScoreHud::setScore( int score )
{
	this->scoreDisplay->clear();

	std::string scoreStr = boost::lexical_cast<std::string>(score);

	// Calculate where to position score so it is center aligned
	float scoreWidth = scoreStr.size() * (this->width * CHAR_WIDTH_PERCENTAGE);
	float scorePosX = (this->width - scoreWidth) / 2;
	scorePosX += this->posX;
	float scorePosY = (this->posY - this->height) + (this->height * CHAR_HEIGHT_PERCENTAGE);
	this->scoreDisplay->addText( scoreStr, scorePosX, scorePosY );
}

void ScoreHud::render()
{
	OGLHudObject::render();
	this->scoreDisplay->render();
}

void ScoreHud::eventHandler( EventArgs* eventArgs )
{
	if ( eventArgs->getEventType() != SCORE_CHANGED_EVENT ) return;

	ScoreChangedEventArgs* scoreChangedEventArgs = dynamic_cast<ScoreChangedEventArgs*>(eventArgs);
	if ( scoreChangedEventArgs != nullptr )
	{
		setScore( scoreChangedEventArgs->getScoreDelta() );
	}
}