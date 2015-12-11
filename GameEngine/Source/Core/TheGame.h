#pragma once
#ifndef THE_GAME
#define THE_GAME

#include <string>
using std::string;
#include <vector>
using std::vector;

class GameEngine;
class ObjectLoader;

class TheGame
{
public:
	GameEngine *gameEngine;
	vector<string> shaderNames;

public:
	TheGame();
	virtual ~TheGame();

	void loadShaders();

	void setupGraphicsParameters();

	void setupViewingEnvironment();

	void sendShaderData();

	void setup(ObjectLoader* loader);

	void processInputs();
};

#endif

