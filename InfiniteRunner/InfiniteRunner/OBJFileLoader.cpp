#include "OBJFileLoader.h"
#include "StringHelper.h"
#include "RGBA.h"
#include "Logger.h"
#include "BMPTextureLoader.h"
#include "OGL2DTexture.h"

#include "LineBox.h"
using namespace std;

OBJFileLoader::OBJFileLoader( TextureManager* texManager, Logger *logger )
{
	this->texManager = texManager;
	this->logger = logger;

	// Create the vectors
	this->vertices = new vector<Vertex>();
	this->normals = new vector<Normal>();
	this->textureCoords = new vector<TextureCoord>();

	this->reset();

}

OBJFileLoader::~OBJFileLoader()
{
	// Cleanup
	this->curGroup = NULL;

	this->reset();

	// Delete vectors
	delete this->vertices;
	delete this->normals;
	delete this->textureCoords;
}

OGL3DGroupObject* OBJFileLoader::loadFile( string filePath, string objName )
{
	this->reset();
	this->logger->log( "Loading OBJ: '" + filePath + "'...\n" );

	this->vertices->reserve( 128 );
	this->normals->reserve( 128 );
	this->textureCoords->reserve( 128 );

	// Attempt to parse the obj file
	if ( !this->parseFile( filePath ) )
	{
		// Abort
		return nullptr;
	}

	this->logger->debug( "Mapping OBJ...\n" );
	BMPTextureLoader texLoader;

	// Are there materials?
	bool hasMats = this->materials.hasData();

	// Create new vectors to hold the combined data from each group
	vector<index_buffer_t> vIndexVector;
	vector<index_buffer_t> nIndexVector;
	vector<index_buffer_t> tIndexVector;
	vector<vertex_buffer_t> bufferVector;

	// Create a vector to hold vertex colors
	vector<RGBA> colorVector;

	// Tracks the number of vertices added
	index_buffer_t vertexCount = 0;

	// Group object
	OGL3DGroupObject * objObject = new OGL3DGroupObject( objName );

	RGBA matColor = { 1,1,1,1 };

	// Minimum and maximum vertice values
	glm::vec3 minVertices( FLT_MAX ), maxVertices( FLT_MIN );

	// Create object per material group
	for each (auto matGroupItem in this->matGroups)
	{
		FaceGroup* group = matGroupItem.second;

		if ( group->indexes->size() == 0 )
		{
			continue;
		}
		this->logger->debug( "Creating sub-object " + group->groupName );

		// Get the group material
		OBJMaterial *gMat = this->materials.getMaterial( group->materialName );
		if ( gMat != nullptr )
		{
			matColor = gMat->diffuse;
		}
		else
		{
			matColor = { 1,1,1,1 };
		}

		// Get the index count
		uint indexCount = group->indexes->size();

		// Calculate the vertex array size. Size * 4(vertex) * 4(color) * 3(normal) * 2(texture)
		uint bufferSize = indexCount * (4 + 4 + 3 + 2);

		// Create the verted & index buffers
		vertex_buffer_t *vertexBuffer = new vertex_buffer_t[bufferSize + 1];
		index_buffer_t *indexBuffer = new index_buffer_t[indexCount + 1];

		// Set the count as the first element in the array
		vertexBuffer[0] = (vertex_buffer_t)bufferSize;
		indexBuffer[0] = (index_buffer_t)indexCount;

		// Create pointers for each section
		Vertex *vertexPtr = (Vertex*)&vertexBuffer[1];
		RGBA *colorPtr = (RGBA*)(vertexPtr + indexCount);
		Normal *normalPtr = (Normal*)(colorPtr + indexCount);
		TextureCoord *texPtr = (TextureCoord*)(normalPtr + indexCount);

		// Add each face
		int vcId = 0, nId = 0, tId = 0, iId = 0;
		for each (IndexData indexData in *group->indexes)
		{
			Vertex vert = (*this->vertices)[indexData.vertex];

			// Add vertex & color
			vertexPtr[vcId] = vert;
			colorPtr[vcId++] = matColor;

			// Add normal
			normalPtr[nId++] = (*this->normals)[indexData.normal];

			// Add texcoord
			texPtr[tId++] = (*this->textureCoords)[indexData.textureCoord];

			// Add indexes
			indexBuffer[1 + iId] = iId++;

			// Bounds check minimums
			if ( vert.x < minVertices.x ) minVertices.x = vert.x;
			if ( vert.y < minVertices.y ) minVertices.y = vert.y;
			if ( vert.z < minVertices.z ) minVertices.z = vert.z;
			// Bounds check maximums
			if ( vert.x > maxVertices.x ) maxVertices.x = vert.x;
			if ( vert.y > maxVertices.y ) maxVertices.y = vert.y;
			if ( vert.z > maxVertices.z ) maxVertices.z = vert.z;

		} // End for each index

		// Create an object
		OGL3DObject *object = new OGL3DObject( group->groupName );

		// Setup the object
		object->createElementArrayPCNT( group->groupName, vertexBuffer, indexBuffer, GL_TRIANGLES );

		if ( gMat != nullptr )
		{
			// Is there a texture path?
			if ( gMat->texturePath != "" )
			{
				// Get the texture from the manager
				OGL2DTexture *texture = (OGL2DTexture*)this->texManager->getTexture( gMat->texturePath );

				// Does the texture need to be loaded?
				if ( texture == nullptr )
				{

					// Set the path
					texLoader.setFilePath( gMat->texturePath );

					// Load texture
					texture = new OGL2DTexture( texLoader.load() );
					texture->setTexelFormat( GL_BGR );
					texture->setTypeOfData( GL_UNSIGNED_BYTE );
					texture->create();

					// Add to manager
					this->texManager->addTexture( gMat->texturePath, texture );
				}

				// Set texture
				object->setTexture( texture );
			}

			// Get specular
			object->material.specular = gMat->specular;
		}
		else
		{
			// Default to white
			object->material.specular = { 1,1,1,1 };
		}

		// Add the object
		objObject->addObject( object );

	} // End for each group

	// Calculate and set the bounding box
	float boundWidth = maxVertices.x - minVertices.x;
	float boundHeight = maxVertices.y - minVertices.y;
	float boundDepth = maxVertices.z - minVertices.z;
	objObject->boundingBox.set( boundWidth, boundDepth, boundHeight );
	objObject->boundingBox.use = true;

	// TODO: Test code for line boxes
	//LineBox *lb = new LineBox( "", boundWidth, boundDepth, boundHeight );
	//lb->setShaderProgram( 6 );
	//objObject->lineBox = lb;


	this->logger->log( "Finished Loading OBJ.\n" );

	return objObject;
}

void OBJFileLoader::reset()
{
	// Delete each group
	for each (auto iterator in this->matGroups)
	{
		// Get the group
		FaceGroup* group = iterator.second;

		// Delete the groups index buffer
		delete group->indexes;

		// Delete the group
		delete group;
	}
	this->groups.clear();
	this->matGroups.clear();
	this->vertices->clear();
	this->normals->clear();
	this->textureCoords->clear();

	this->materials.reset();

	// Create a default group
	this->createAndSetGroup( this->NO_GROUP );
	this->matGroups[this->NO_MATS] = this->curGroup;

	// Set default values
	this->vertices->push_back( Vertex{ 0,0,0 } );
	this->normals->push_back( Normal{ 0,0,1 } );
	this->textureCoords->push_back( TextureCoord{ 0,0 } );
}

void OBJFileLoader::createAndSetGroup( string groupName )
{
	// Create the group
	FaceGroup *group = new FaceGroup();
	group->groupName = groupName;
	group->indexes = new vector<IndexData>();
	group->materialName = "";

	// Add to the vector
	this->groups.push_back( group );

	// Set as current
	this->curGroup = group;
}

bool OBJFileLoader::parseFile( string filePath )
{
	// True if everything is OK, no errors
	bool ok = true;

	// True if a material file is found and colors are used
	bool hasColors = false;

	// Stores the results of split
	vector<CharIterator> lines, tokens;

	// Open the file
	CharIterator fileData = CharIterator::FromFile( filePath );

	// Could the file not be opened?
	if ( fileData.getLength() == 0 )
	{
		// Write error
		this->logger->log( "Unable to open OBJ file\n" );

		// Abort
		return false;
	}

	// Split the file data into lines
	lines.reserve( 1024 );
	fileData.split( (char)10, &lines );

	// Holds the names
	string tmpNames = "";

	// Setup the material group holder
	auto matGroup = this->matGroups.end();

	// Read each line
	CharIterator line;

	for ( uint lineNumber = 0; lineNumber < lines.size(); ++lineNumber )
	{
		line = lines[lineNumber];


		// Is the line empty?
		if ( line.getLength() == 0 )
		{
			// Skip empty lines
			continue;
		}

		// Split the line
		tokens.clear();
		line.split( ' ', &tokens );

		// Determine line type
		switch ( line[0] )
		{
			// v, vn, vt
		case 'v':
			switch ( line[1] )
			{
				// v - vertex
			case 0:
				ok = this->parseVertices( tokens );
				break;
				// n - normal
			case 'n':
				ok = this->parseNormals( tokens );
				break;
				// t - texture coord
			case 't':
				ok = this->parseTextureCoords( tokens );
				break;
			}
			break;

			// f - face
		case 'f':
			ok = this->parseFace( tokens );
			break;

			// g - group
		case 'g':
			// Get the group name
			tmpNames = tokens[1].toString();

			// If there are no materials, there is no reason to group.
			if ( this->materials.hasData() )
			{
				this->createAndSetGroup( tmpNames );
				this->logger->debug( "Loading group: " + tmpNames );
			}
			else
			{
				this->logger->debug( "Merging group: " + tmpNames );
			}
			break;

			// usemtl
		case 'u':
			if ( this->materials.hasData() )
			{
				// Get the material name
				tmpNames = tokens[1].toString();

				// Does the map have this material?
				matGroup = this->matGroups.find( tmpNames );
				if ( matGroup != this->matGroups.end() )
				{
					if ( matGroup->second != this->curGroup )
					{
						// Delete the current group, it should be empty if a new group was just started.
						// This assumes the OBJ standard is being followed where a group is defined
						// then on the next line it's material is set.
						delete this->curGroup;

						// Set the current group
						this->curGroup = matGroup->second;
					}
				}
				else
				{
					// Add to the map
					this->matGroups[tmpNames] = this->curGroup;

					if ( this->matGroups[this->NO_MATS] == this->curGroup )
					{
						this->matGroups.erase( this->NO_MATS );
					}

					// Set the material
					this->curGroup->materialName = tmpNames;
				}
			}
			break;

			// mtllib
		case 'm':
			// Load material file
			this->materials.loadFile( tokens[1].toString() );

			break;
		}

		// Is everything ok?
		if ( !ok )
		{
			break;
		}

	}

	// Cleanup
	delete[] fileData.getSource();

	return ok;
}

bool OBJFileLoader::parseVertices( vector<CharIterator> &tokens )
{
	// There must be at least 4 items
	if ( tokens.size() < 4 )
	{
		// Invalid number of tokens
		return false;
	}

	// Add vertex
	this->vertices->push_back(
		Vertex{ tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat(), 1.0f }
	);

	// Done
	return true;
}

bool OBJFileLoader::parseNormals( vector<CharIterator> &tokens )
{
	// There must be at least 4 items
	if ( tokens.size() < 4 )
	{
		// Invalid number of tokens
		return false;
	}

	// Add vertex
	this->normals->push_back(
		Normal{ tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat() }
	);

	// Done
	return true;
}

bool OBJFileLoader::parseTextureCoords( vector<CharIterator> &tokens )
{
	// There must be at least 3 items
	if ( tokens.size() < 3 )
	{
		// Invalid number of tokens
		return false;
	}

	// Add vertex
	this->textureCoords->push_back(
		TextureCoord{ tokens[1].toFloat(), tokens[2].toFloat() }
	);

	// Done
	return true;
}

bool OBJFileLoader::parseFace( vector<CharIterator> &tokens )
{
	// Get the number of indexes in this face
	int polySize = (tokens.size() - 1);

	// There must be at least 3 indexes
	if ( polySize < 3 )
	{
		// Invalid number of tokens
		return false;
	}

	// Temp storage for the indexes
	IndexData *indexTemp = new IndexData[polySize];

	vector<CharIterator> subTokens;
	index_buffer_t vToken, nToken, tToken;

	// Loop over tokens
	for ( int i = 0; i < polySize; ++i )
	{
		vToken = 0;
		tToken = 0;
		nToken = 0;

		// Split
		subTokens.clear();
		tokens[i + 1].split( '/', &subTokens );

		// Read the vertex
		vToken = subTokens[0].toUInt();

		// Vertex + Texture: v/t
		if ( subTokens.size() == 2 )
		{
			// Read the texture
			tToken = subTokens[1].toUInt();
		}
		else if ( subTokens.size() == 3 )
		{
			// Vertex + Texture + Normal: v/t/n
			if ( subTokens[1].getLength() > 0 )
			{
				// Read the texture
				tToken = subTokens[1].toUInt();
			}

			// Read the normal
			nToken = subTokens[2].toUInt();
		}

		// Set indexes
		indexTemp[i] = { vToken, nToken, tToken };

		// Bounds check
		if ( vToken < 0 )
		{
			// Vertex index out of bounds
			this->logger->log( "Vertex Index Out Of Bounds: " + vToken );
			return false;
		}
	}

	int count = 0;
	for ( int arrIndex = 0; arrIndex < polySize; ++arrIndex )
	{
		// Inc the count, and check if triangle finished
		if ( ++count == 4 )
		{
			// Push the last index again
			this->curGroup->indexes->push_back( indexTemp[arrIndex - 1] );

			// Reset count
			count = 2;
		}

		// Push the indexs
		this->curGroup->indexes->push_back( indexTemp[arrIndex] );
	}

	// Check for origin connect
	if ( count < 3 )
	{
		// Push origin index
		this->curGroup->indexes->push_back( indexTemp[0] );
	}

	// Cleanup
	delete[] indexTemp;

	// Done
	return true;
}

