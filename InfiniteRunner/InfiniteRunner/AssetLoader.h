#pragma once
#ifndef ASSET_LOADER
#define ASSET_LOADER

#include <string>
#include <vector>

#include "BaseObject.h"

class GameEngine;

class AssetLoader : public BaseObject
{
private:
	std::vector<std::string> lines;
	size_t ci;

protected:
	GameEngine* gameEngine;

public:
	AssetLoader();

	virtual ~AssetLoader();

	virtual void setGameEngine( GameEngine* gameEngine )
	{
		this->gameEngine = gameEngine;
	}

	virtual void loadAssetFile( const std::string& filepath );

	virtual void loadAssets() = 0;

	virtual void setAtStart() { this->ci = 0; }

	virtual bool isFileLoaded() { return this->lines.size() > 0; }

protected:
	std::string nextLine();

	std::string currLine() { return this->lines[this->ci]; }

	bool atEnd() { return this->ci == this->lines.size() - 1; }

	void split( std::vector<std::string>& fields, const std::string& record, char delimeter = ',' );

	void trim( std::string& str );

	void loadElement( std::vector<std::string>& fields, const std::string& end, char delimeter = ',' );
};

#endif

