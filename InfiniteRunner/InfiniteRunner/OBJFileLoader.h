#pragma once
#include "OBJMaterialLoader.h"
#include "EngineTypes.h"
#include "OGL3DGroupObject.h"
#include "CharIterator.h"
#include "TextureManager.h"

#include <string>
#include <vector>
#include <map>

class Logger;

class OBJFileLoader
{
private:

	/*
	Vertex data.
	*/
	struct Vertex
	{
		vertex_buffer_t x;
		vertex_buffer_t y;
		vertex_buffer_t z;
		vertex_buffer_t w;
	};

	/*
	Normal data.
	*/
	struct Normal
	{
		vertex_buffer_t x;
		vertex_buffer_t y;
		vertex_buffer_t z;
	};

	/*
	Texture data
	*/
	struct TextureCoord
	{
		vertex_buffer_t u;
		vertex_buffer_t v;
	};

	/*
	Vertex, normal, and texture indexes.
	*/
	struct IndexData
	{
		index_buffer_t vertex;
		index_buffer_t normal;
		index_buffer_t textureCoord;
	};

	/*
	Face indices.
	*/
	struct FaceGroup
	{
		std::string groupName;
		std::string materialName;
		std::vector<IndexData> *indexes;
	};

	const std::string NO_MATS = "_no_material_";
	const std::string NO_GROUP = "_no_group_";


	/*
	Holds loaded vertices
	*/
	std::vector<Vertex> *vertices;

	/*
	Holds loaded normals
	*/
	std::vector<Normal> *normals;

	/*
	Holds loaded texture coords
	*/
	std::vector<TextureCoord> *textureCoords;

	/*
	Holds each group
	*/
	std::vector<FaceGroup*> groups;

	std::map<std::string, FaceGroup*> matGroups;

	/*
	The current group
	*/
	FaceGroup *curGroup;

	/*
	Material loader
	*/
	OBJMaterialLoader materials;

	/*
	Logger
	*/
	Logger *logger;

	/*
	Texture manager.
	*/
	TextureManager* texManager;

public:

	/*
	Constructor.
	*/
	OBJFileLoader( TextureManager* texManager, Logger *logger );

	/*
	Destructor.
	*/
	~OBJFileLoader();

	/*
	Loads an OBJ file.
	*/
	OGL3DGroupObject* loadFile( std::string filePath, std::string objName );

private:

	/*
	Resets all internal information.
	*/
	void reset();

	/*
	Creates a new group and set's it to the current group.
	*/
	void createAndSetGroup( std::string groupName );

	/*
	Attempts to parse the obj file.
	Returns true if succeeded.
	*/
	bool parseFile( std::string filePath );

	/*
	Attempts to parse vertex data from the tokens.
	Returns true if succeeded.
	*/
	bool parseVertices( std::vector<CharIterator> &tokens );

	/*
	Attempts to parse normal data from the tokens.
	Returns true if succeeded.
	*/
	bool parseNormals( std::vector<CharIterator> &tokens );

	/*
	Attempts to parse texture data from the tokens.
	Returns true if succeeded.
	*/
	bool parseTextureCoords( std::vector<CharIterator> &tokens );

	/*
	Attempts to parse face data from the tokens.
	Returns true if succeeded.
	*/
	bool parseFace( std::vector<CharIterator> &tokens );
};

