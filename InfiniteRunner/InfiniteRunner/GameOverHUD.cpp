#include "GameOverHUD.h"
#include "GameOverEventArgs.h"
#include <boost\lexical_cast.hpp>

#define CHAR_WIDTH_PERCENTAGE 0.14285f
#define CHAR_HEIGHT_PERCENTAGE 0.333333f

GameOverHUD::GameOverHUD( float width, float height, float posX, float posY, RGBA color ) :
	OGLHudObject( "HealthHud", width, height, posX, posY, color )
{
	OGLHudObject::setHeader( "Press r to restart" );
	// Calculate character dimensions
	float charWidth = width * CHAR_WIDTH_PERCENTAGE;
	float charHeight = height * CHAR_HEIGHT_PERCENTAGE;
	this->scoreDisplay = new OGL2DTextDisplay( charWidth, charHeight );
	this->scoreHeader = new OGL2DTextDisplay( charWidth, charHeight );
	scoreHeader->clear();
	//scoreHeader->addText("Score:", (this->width - 6) / 2 +  this->posX, this->posY - (CHAR_HEIGHT_PERCENTAGE / 2.5f));
	scoreHeader->addText( "Score:", -(this->width / 2.0f) + 0.06, 0.05f );
	this->visible = false;
}

GameOverHUD::~GameOverHUD()
{
	delete this->scoreDisplay;
	delete this->scoreHeader;
}

void GameOverHUD::setTextTexture( OGL2DTexture* texture )
{
	this->scoreDisplay->setTexture( texture );
	this->scoreHeader->setTexture( texture );
	OGLHudObject::setTextTexture( texture );
}

void GameOverHUD::setTextShaderProgram( GLuint shaderProgram )
{
	this->scoreDisplay->setShaderProgram( shaderProgram );
	this->scoreHeader->setShaderProgram( shaderProgram );
	OGLHudObject::setTextShaderProgram( shaderProgram );
}

void GameOverHUD::setScore( int score )
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

void GameOverHUD::render()
{
	if ( this->visible )
	{
		OGLHudObject::render();
		//TODO: why isn't this rendering
		this->scoreHeader->render();
		this->scoreDisplay->render();
	}
}

void GameOverHUD::eventHandler( EventArgs* eventArgs )
{
	switch ( eventArgs->getEventType() )
	{
	case GAME_OVER_EVENT:
	{
		GameOverEventArgs* gameOverEventArgs = dynamic_cast<GameOverEventArgs*>(eventArgs);
		if ( gameOverEventArgs != nullptr )
		{
			setScore( gameOverEventArgs->getScore() );
			this->visible = true;
		}
	}
	break;
	case GAME_RESET_EVENT:
	{
		this->visible = false;
	}
	break;
	}

}