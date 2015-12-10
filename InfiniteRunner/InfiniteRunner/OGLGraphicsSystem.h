#pragma once
#ifndef OGL_GRAPHICS_SYSTEM
#define OGL_GRAPHICS_SYSTEM

#include "GraphicsSystem.h"

#include <gl\glew.h>
#include <string>

class OGLShaderManager;
class OGLVertexShader;
class OGLFragmentShader;
class OGLShaderProgram;
class GameWorld;
class OGLViewingFrustum;
class OGLCamera;
class GameObject;
class OGL2DTextDisplay;
class TextureManager;
class OGLHudObject;

class OGLGraphicsSystem : public GraphicsSystem
{
protected:
	OGLShaderManager *oglShaderManager;
	OGLViewingFrustum *oglFrustum;
	OGLCamera *oglCamera;
	OGL2DTextDisplay *textDisplay;
	std::vector<OGLHudObject*> hudObjects;

public:
	OGLGraphicsSystem(
		OGLShaderManager *shaderManager,
		GameWorld *gameWorld,
		OGLViewingFrustum *oglFrustum,
		TextureManager *textureManager );

	virtual ~OGLGraphicsSystem();

	OGLShaderManager *getOGLShaderManager()
	{
		return (OGLShaderManager*)this->shaderManager;
	}

	void addVertexShader(
		const std::string& name,
		OGLVertexShader *shader,
		const std::string& sourceCode = "" );

	void addFragmentShader(
		const std::string& name,
		OGLFragmentShader *shader,
		const std::string& sourceCode = "" );

	void addShaderProgram(
		const std::string& name,
		OGLShaderProgram *shaderProgram,
		const std::string& vertexShaderName,
		const std::string& fragmentShaderName );

	void addHudObject( OGLHudObject* hudObject ) { this->hudObjects.push_back( hudObject ); }

	GameObject* getGameObject( const std::string& name );

	void setTextDisplay( OGL2DTextDisplay *textDisplay ) { this->textDisplay = textDisplay; }

	bool initialize();

	void setUpViewingEnvironment();

	void clearViewPort(
		unsigned int left, unsigned int top,
		unsigned int width, unsigned int height,
		RGBA color );

	void update( float elapsedSeconds );

	void render();

	void setObjectsShaderProgram( const std::string& shaderProgramName );

	void setObjectShaderProgram( const std::string& objectName, const std::string& shaderProgramName );
};

#endif

