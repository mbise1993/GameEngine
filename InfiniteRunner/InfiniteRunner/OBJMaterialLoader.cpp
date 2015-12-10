#include "OBJMaterialLoader.h"
#include "StringHelper.h"

#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;

OBJMaterial::OBJMaterial()
{
	this->ambient =
		this->specular =
		this->diffuse = { 1,1,1,1 };
	this->texturePath = "";
}

OBJMaterialLoader::OBJMaterialLoader()
{
	this->isValid = false;
}


OBJMaterialLoader::~OBJMaterialLoader()
{
	this->reset();
}

void OBJMaterialLoader::reset()
{
	this->isValid = false;
	for ( auto i = this->materialMap.begin(); i != this->materialMap.end(); i++ )
	{
		delete i->second;
	}
	this->materialMap.clear();
}

void OBJMaterialLoader::loadFile( std::string filePath )
{
	// Open the file
	ifstream stream;
	stream.open( filePath, ios::in );

	// Could the file not be opened?
	if ( stream.fail() )
	{
		// Write error
		//printf( "Unable to open Material file: %s\n", filePath.c_str() );
		//printf( "\tNo color data will be stored.\n" );
		return;
	}
	//printf( "Loading Material file: %s\n", filePath.c_str() );

	// Holds one line
	string line;

	// Stores the results of split
	vector<string> tokens;
	char delim[] = " ";

	string groupName, token;

	// Read each line
	while ( !stream.eof() )
	{
		// Read the next line
		getline( stream, line );

		// Is the line empty?
		if ( StringHelper::isEmptyOrWhiteSpace( line ) )
		{
			// Skip empty lines
			continue;
		}

		// Split the line
		tokens.clear();
		StringHelper::split( line, delim, &tokens );

		token = tokens[0];
		boost::algorithm::to_lower( token );

		// Look for new materials
		if ( token == "newmtl" )
		{
			groupName = StringHelper::join( &tokens, " ", 1 );
			this->materialMap[groupName] = new OBJMaterial();
		}
		// Look for diffuse color
		else if ( token == "kd" )
		{
			// Read colors
			// 1 = red
			float red = stof( tokens[1] );
			// 2 = green
			float green = stof( tokens[2] );
			// 3 = blue
			float blue = stof( tokens[3] );

			// Add to map
			this->materialMap[groupName]->diffuse = RGBA{ red, green, blue, 1.0f };
		}
		// Look for ambient color
		else if ( token == "ka" )
		{
			// Read colors
			// 1 = red
			float red = stof( tokens[1] );
			// 2 = green
			float green = stof( tokens[2] );
			// 3 = blue
			float blue = stof( tokens[3] );

			// Add to map
			this->materialMap[groupName]->ambient = RGBA{ red, green, blue, 1.0f };
		}
		// Look for specular color
		else if ( token == "ks" )
		{
			// Read colors
			// 1 = red
			float red = stof( tokens[1] );
			// 2 = green
			float green = stof( tokens[2] );
			// 3 = blue
			float blue = stof( tokens[3] );

			// Add to map
			this->materialMap[groupName]->specular = RGBA{ red, green, blue, 1.0f };
		}
		else if ( token == "map_kd" )
		{
			this->materialMap[groupName]->texturePath = tokens[1];
		}
	}

	//printf( "Finished loading Material file\n" );
	this->isValid = true;
}

OBJMaterial * OBJMaterialLoader::getMaterial( std::string materialName )
{
	auto item = this->materialMap.find( materialName );

	// Was the not item found?
	if ( item == this->materialMap.end() )
	{
		// Return null
		return nullptr;
	}

	// Return the material
	return item->second;
}
