#pragma once
#ifndef THE_GAME
#define THE_GAME

#include "BaseObject.h"

#include <string>

class GameEngine;
class AssetLoader;

class OGLGraphicsSystem;
class GameObjectManager;
class TextureManager;
class OGLShaderManager;

class TheGame : public BaseObject
{
public:
	GameEngine *gameEngine;

public:
	TheGame();

	virtual ~TheGame();

	void loadShaders( const std::string& shaderAssetFilename );

	void setupGraphicsParameters( const std::string& uniformAssetFilename );

	void setupViewingEnvironment();

	void sendShaderData();

	void setup( const std::string& gameAssetFilename );

	void processInputs();
};

#endif

