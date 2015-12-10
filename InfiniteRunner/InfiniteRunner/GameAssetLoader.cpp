#include "GameAssetLoader.h"
#include "TextureManager.h"
#include "BMPTextureLoader.h"
#include "OGL2DTexture.h"
#include "OGLObject.h"
#include "GameObjectManager.h"
#include "Axis.h"
#include "OGLTexturedFlatSurface.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "OGLShaderManager.h"
#include "GameEngine.h"
#include "OGLGraphicsSystem.h"
#include "GameWorld.h"
#include "OGLVertexShader.h"
#include "TextFileReader.h"
#include "CoreSystem.h"
#include "OGLFragmentShader.h"
#include "Uniform.h"
#include "LightSource.h"
#include "TexturedCuboid.h"
#include "LineBox.h"
#include "OGL3DGroupObject.h"
#include "OBJFileLoader.h"
#include "OGLViewingFrustum.h"
#include "BackForthBehavior.h"
#include "RotateYBehavior.h"
#include "PlayerController.h"

using std::string;
using std::vector;

GameAssetLoader::GameAssetLoader()
{}

GameAssetLoader::~GameAssetLoader()
{}

void GameAssetLoader::loadAssets()
{
	GameObjectManager* gameObjectManager =
		this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();
	this->setAtStart();
	if ( this->currLine() != "<assets>" ) return;
	while ( !this->atEnd() && this->nextLine() != "</assets>" )
	{
		if ( this->currLine() == "<textures>" )
		{
			this->loadTextures();
		}
		else if ( this->currLine() == "<gameobjects>" )
		{
			this->loadGameObjects();
		}
		else if (this->currLine() == "<chunkSystemData>")
		{
			this->loadChunkSystemData();
		}
	}
}

void GameAssetLoader::loadOnlyTheShaders()
{
	this->setAtStart();
	if ( this->currLine() != "<assets>" ) return;
	while ( !this->atEnd() && this->nextLine() != "</assets>" )
	{
		if ( this->currLine() == "<shaders>" )
		{
			this->loadShaders();
			break;
		}
	}
}

void GameAssetLoader::loadOnlyTheUniforms()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader;
	shader = (OGLShaderProgram*)shaderMgr->getShader( "SimpleShader" );
	this->setAtStart();
	if ( this->currLine() != "<assets>" ) return;
	while ( !this->atEnd() && this->nextLine() != "</assets>" )
	{
		if ( this->currLine() == "<uniforms>" )
		{
			this->loadUniforms();
			break;
		}
	}
}

void GameAssetLoader::loadShaders()
{
	while ( !this->atEnd() && this->nextLine() != "</shaders>" )
	{
		if ( this->currLine() == "<vertexShaders>" )
		{
			this->loadVertexShaders();
		}
		else if ( this->currLine() == "<fragmentShaders>" )
		{
			this->loadFragmentShaders();
		}
		else if ( this->currLine() == "<shaderPrograms>" )
		{
			this->loadShaderPrograms();
		}
	}
}

void GameAssetLoader::loadVertexShaders()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	CoreSystem* core = this->gameEngine->getCoreSystem();
	TextFileReader *reader = core->getTextFileReader();

	string shaderName;
	string fileName;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</vertexShaders>" )
	{
		this->split( fields, this->currLine() );
		shaderName = fields[0];
		switch ( fields.size() )
		{
		case 1:
			graphics->addVertexShader(
				shaderName,
				new OGLVertexShader()
				);
			break;
		case 2:
			fileName = fields[1];
			graphics->addVertexShader(
				shaderName,
				new OGLVertexShader(),
				reader->readContents( fileName )
				);
			break;
		}
		fields.clear();
	}
}

void GameAssetLoader::loadFragmentShaders()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	CoreSystem* core = this->gameEngine->getCoreSystem();
	TextFileReader *reader = core->getTextFileReader();

	string shaderName;
	string fileName;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</fragmentShaders>" )
	{
		this->split( fields, this->currLine() );
		shaderName = fields[0];
		switch ( fields.size() )
		{
		case 1:
			graphics->addFragmentShader(
				shaderName,
				new OGLFragmentShader()
				);
			break;
		case 2:
			fileName = fields[1];
			graphics->addFragmentShader(
				shaderName,
				new OGLFragmentShader(),
				reader->readContents( fileName )
				);
			break;
		}
		fields.clear();
	}
}

void GameAssetLoader::loadShaderPrograms()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();

	string shaderName, vertexName, fragmentName;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</shaderPrograms>" )
	{
		this->split( fields, this->currLine() );
		shaderName = fields[0];
		vertexName = fields[1];
		fragmentName = fields[2];
		graphics->addShaderProgram(
			shaderName,
			new OGLShaderProgram(),
			vertexName, fragmentName
			);
		fields.clear();
	}
}

void GameAssetLoader::loadUniforms()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader;

	string shaderName, uniformName, uniformTypeName;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</uniforms>" )
	{
		this->split( fields, this->currLine() );
		shaderName = fields[0];
		uniformName = fields[1];
		uniformTypeName = fields[2];
		shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );
		shader->addUniform( uniformName, Uniform::getType( uniformTypeName ) );
		fields.clear();
	}
}

void GameAssetLoader::loadGameObjects()
{
	OGL3DObject* object = nullptr;
	GameObjectManager* objManager = (GameObjectManager*)this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();

	while ( !this->atEnd() && this->nextLine() != "</gameobjects>" )
	{
		if ( this->currLine() == "<skybox>" )
		{
			this->loadSkybox();
		}
		else if ( this->currLine() == "<axis>" )
		{
			this->loadAxis();
		}
		else if ( this->currLine() == "<otfs>" )
		{
			this->loadOTFS();
		}
		else if ( this->currLine() == "<localLightSources>" )
		{
			this->loadLocalLightSources();
		}
		else if ( this->currLine() == "<texturedCuboid>" )
		{
			this->loadTexturedCuboid();
		}
		else if ( this->currLine() == "<sphere>" )
		{
			this->loadSphere();
		}
		else if ( this->currLine() == "<cuboid>" )
		{
			this->loadCuboid();
		}
		else if ( this->currLine() == "<linebox>" )
		{
			this->loadLineBox();
		}
		else if (this->currLine() == "<objFile>")
		{
			object = this->loadObjFile();
		}

		if (object)
		{
			objManager->addObject(object->getName(), object);
			object = nullptr;
		}
	}
}

void GameAssetLoader::loadChunkSystemData()
{
	ChunkSettings* chunkSettings = new ChunkSettings();
	chunkSettings->renderDistance_zFar = this->gameEngine->getGraphicsSystem()->getViewingFrustum()->getZFar();

	ChunkGenerator* chunkGenerator = nullptr;

	while (!this->atEnd() && this->nextLine() != "</chunkSystemData>")
	{
		if (this->currLine() == "<chunkSettings>")
		{
			this->loadChunkSettings(chunkSettings);
			chunkGenerator = new ChunkGenerator(chunkSettings);
		}
		else if (this->currLine() == "<chunkObjects>")
		{
			this->loadChunkObjects(chunkGenerator);
		}
	}

	GameObjectManager* objManager = (GameObjectManager*)this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();

	ChunkSystem* chunkSystem = new ChunkSystem(chunkSettings, this->logger);
	chunkSystem->setGenerator(chunkGenerator);
	objManager->setChunkSystem(chunkSystem);
}

void GameAssetLoader::loadChunkSettings(ChunkSettings* chunkSettings)
{
	std::string::size_type sz;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</chunkSettings>")
	{
		if (this->currLine() == "<chunkDimensions>")
		{
			this->loadElement(fields, "</chunkDimensions>");
			chunkSettings->chunkDimensions = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<rowRadius>")
		{
			this->loadElement(fields, "</rowRadius>");
			chunkSettings->rowRadius = std::stof(fields[0], &sz);
		}
		if (this->currLine() == "<generationDensity>")
		{
			this->loadElement(fields, "</generationDensity>");
			chunkSettings->generationDensity = std::stof(fields[0], &sz);
		}
		fields.clear();
	}
}

void GameAssetLoader::loadChunkObjects(ChunkGenerator* chunkGenerator)
{
	OGL3DObject* object = nullptr;

	while (!this->atEnd() && this->nextLine() != "</chunkObjects>")
	{
		if (this->currLine() == "<texturedCuboid>")
		{
			this->loadTexturedCuboid();
		}
		else if (this->currLine() == "<sphere>")
		{
			this->loadSphere();
		}
		else if (this->currLine() == "<cuboid>")
		{
			this->loadCuboid();
		}
		else if (this->currLine() == "<linebox>")
		{
			this->loadLineBox();
		}
		else if (this->currLine() == "<objFile>")
		{
			object = this->loadObjFile();
		}

		if (object)
		{
			chunkGenerator->addObjectToPool(object);
			object = nullptr;
		}
	}
}

void GameAssetLoader::loadSkybox()
{
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float size;
	string shaderName;
	std::map<SkyboxSide, string> texNames;
	while ( !this->atEnd() && this->nextLine() != "</skybox>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<size>" )
		{
			this->loadElement( fields, "</size>" );
			size = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<textureNames>" )
		{
			this->loadSkyboxTextureNames( texNames );
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameWorld* gameWorld = (GameWorld*)graphics->getGameWorld();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	Skybox* skybox = new Skybox( name, size );
	for ( auto texName : texNames )
	{
		Texture* texture = textureMgr->getTexture( texName.second );
		skybox->setTexture( texName.first, texture );
	}

	skybox->setShaderProgram( shader->getHandle() );
	gameWorld->setSkybox( skybox );
}

void GameAssetLoader::loadSkyboxTextureNames( std::map<SkyboxSide, string>& texNames )
{
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</textureNames>" )
	{
		if ( this->currLine() == "<bottom>" )
		{
			this->loadElement( fields, "</bottom>" );
			texNames[SKYBOX_BOTTOM] = fields[0];
		}
		if ( this->currLine() == "<top>" )
		{
			this->loadElement( fields, "</top>" );
			texNames[SKYBOX_TOP] = fields[0];
		}
		if ( this->currLine() == "<front>" )
		{
			this->loadElement( fields, "</front>" );
			texNames[SKYBOX_FRONT] = fields[0];
		}
		if ( this->currLine() == "<back>" )
		{
			this->loadElement( fields, "</back>" );
			texNames[SKYBOX_BACK] = fields[0];
		}
		if ( this->currLine() == "<left>" )
		{
			this->loadElement( fields, "</left>" );
			texNames[SKYBOX_LEFT] = fields[0];
		}
		if ( this->currLine() == "<right>" )
		{
			this->loadElement( fields, "</right>" );
			texNames[SKYBOX_RIGHT] = fields[0];
		}
		fields.clear();
	}
}

void GameAssetLoader::loadAxis()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float length = 1;
	string shaderName;

	while ( !this->atEnd() && this->nextLine() != "</axis>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<length>" )
		{
			this->loadElement( fields, "</length>" );
			length = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new Axis( name, length );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

void GameAssetLoader::loadOTFS()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 5, depth = 5;
	unsigned int widthSegments = 5, depthSegments = 5;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;

	while ( !this->atEnd() && this->nextLine() != "</otfs>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<width>" )
		{
			this->loadElement( fields, "</width>" );
			width = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<depth>" )
		{
			this->loadElement( fields, "</depth>" );
			depth = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<widthSegments>" )
		{
			this->loadElement( fields, "</widthSegments>" );
			widthSegments = std::stoi( fields[0], &sz );
		}
		else if ( this->currLine() == "<depthSegments>" )
		{
			this->loadElement( fields, "</depthSegments>" );
			depthSegments = std::stoi( fields[0], &sz );
		}
		else if ( this->currLine() == "<color>" )
		{
			this->loadElement( fields, "</color>" );
			color = {
				std::stof( fields[0], &sz ),
				std::stof( fields[1], &sz ),
				std::stof( fields[2], &sz ),
				std::stof( fields[3], &sz )
			};
		}
		else if ( this->currLine() == "<textureName>" )
		{
			this->loadElement( fields, "</textureName>" );
			textureName = fields[0];
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new OGLTexturedFlatSurface( name, width, depth, widthSegments, depthSegments, color );
	Texture* texture = textureMgr->getTexture( textureName );
	object->setTexture( texture );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

void GameAssetLoader::loadTexturedCuboid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;

	while ( !this->atEnd() && this->nextLine() != "</texturedCuboid>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<width>" )
		{
			this->loadElement( fields, "</width>" );
			width = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<depth>" )
		{
			this->loadElement( fields, "</depth>" );
			depth = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<height>" )
		{
			this->loadElement( fields, "</height>" );
			height = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<color>" )
		{
			this->loadElement( fields, "</color>" );
			color = {
				std::stof( fields[0], &sz ),
				std::stof( fields[1], &sz ),
				std::stof( fields[2], &sz ),
				std::stof( fields[3], &sz )
			};
		}
		else if ( this->currLine() == "<textureName>" )
		{
			this->loadElement( fields, "</textureName>" );
			textureName = fields[0];
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new TexturedCuboid( name, width, depth, height, color );
	Texture* texture = textureMgr->getTexture( textureName );
	object->setTexture( texture );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

void GameAssetLoader::loadCuboid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;

	while ( !this->atEnd() && this->nextLine() != "</cuboid>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<width>" )
		{
			this->loadElement( fields, "</width>" );
			width = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<depth>" )
		{
			this->loadElement( fields, "</depth>" );
			depth = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<height>" )
		{
			this->loadElement( fields, "</height>" );
			height = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<color>" )
		{
			this->loadElement( fields, "</color>" );
			color = {
				std::stof( fields[0], &sz ),
				std::stof( fields[1], &sz ),
				std::stof( fields[2], &sz ),
				std::stof( fields[3], &sz )
			};
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new Cuboid( name, width, depth, height, color );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

void GameAssetLoader::loadLineBox()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;

	while ( !this->atEnd() && this->nextLine() != "</linebox>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<width>" )
		{
			this->loadElement( fields, "</width>" );
			width = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<depth>" )
		{
			this->loadElement( fields, "</depth>" );
			depth = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<height>" )
		{
			this->loadElement( fields, "</height>" );
			height = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<color>" )
		{
			this->loadElement( fields, "</color>" );
			color = {
				std::stof( fields[0], &sz ),
				std::stof( fields[1], &sz ),
				std::stof( fields[2], &sz ),
				std::stof( fields[3], &sz )
			};
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new LineBox( name, width, depth, height, color );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

void GameAssetLoader::loadSphere()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float radius = 1;
	int slices = 10, stacks = 10;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;

	while ( !this->atEnd() && this->nextLine() != "</sphere>" )
	{
		if ( this->currLine() == "<name>" )
		{
			this->loadElement( fields, "</name>" );
			name = fields[0];
		}
		else if ( this->currLine() == "<radius>" )
		{
			this->loadElement( fields, "</radius>" );
			radius = std::stof( fields[0], &sz );
		}
		else if ( this->currLine() == "<slices>" )
		{
			this->loadElement( fields, "</slices>" );
			slices = std::stoi( fields[0], &sz );
		}
		else if ( this->currLine() == "<stacks>" )
		{
			this->loadElement( fields, "</stacks>" );
			stacks = std::stoi( fields[0], &sz );
		}
		else if ( this->currLine() == "<color>" )
		{
			this->loadElement( fields, "</color>" );
			color = {
				std::stof( fields[0], &sz ),
				std::stof( fields[1], &sz ),
				std::stof( fields[2], &sz ),
				std::stof( fields[3], &sz )
			};
		}
		else if ( this->currLine() == "<shaderName>" )
		{
			this->loadElement( fields, "</shaderName>" );
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader( shaderName );

	object = new Sphere( name, radius, slices, stacks, color );
	object->setShaderProgram( shader->getHandle() );
	objectMgr->addObject( name, object );
}

OGL3DObject* GameAssetLoader::loadObjFile()
{
	OGL3DObject* object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	string fileName;
	float speed = 0.0f;
	std::vector<TransformDefinition> transforms;
	IBehavior* behavior = nullptr;
	BoundingBox bb;
	string shaderName;

	while (!this->atEnd() && this->nextLine() != "</objFile>")
	{
		if (this->currLine() == "<name>")
		{
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<fileName>")
		{
			this->loadElement(fields, "</fileName>");
			fileName = fields[0];
		}
		else if (this->currLine() == "<speed>")
		{
			this->loadElement(fields, "</speed>");
			speed = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<transforms>")
		{
			this->loadTransforms(transforms);
		}
		else if (this->currLine() == "<behavior>")
		{
			behavior = this->loadBehavior();
		}
		else if (this->currLine() == "<boundingBox>")
		{
			// TODO: This is now deprecated
			//this->loadBoundingBox(bb);
		}
		else if (this->currLine() == "<shaderName>")
		{
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);

	OBJFileLoader loader{ textureMgr, this->logger };
	object = loader.loadFile(fileName, name);

	if (object)
	{
		object->setShaderProgram(shader->getHandle());
		object->setVisibility(true);
		object->setSpeed(speed);
		
		// TODO: Deprecated
		//object->boundingBox = bb;

		// Apply all transforms
		for (auto transform : transforms)
		{
			switch (transform.type)
			{
			case SCALE:
				object->referenceFrame.scaleBy(transform.value);
				break;
			case ROTATE_X:
				object->referenceFrame.rotateX(transform.value);
				break;
			case ROTATE_Y:
				object->referenceFrame.rotateY(transform.value);
				break;
			case ROTATE_Z:
				object->referenceFrame.rotateZ(transform.value);
				break;
			}
		}

		// If behavior was provided, apply it
		if (behavior)
		{
			object->setBehavior(behavior);
		}
	}

	return object;
}

void GameAssetLoader::loadTransforms(std::vector<TransformDefinition>& transforms)
{
	std::string::size_type sz;
	vector<string> fields;

	while (!this->atEnd() && this->nextLine() != "</transforms>")
	{
		if (this->currLine() == "<scale>")
		{
			this->loadElement(fields, "</scale>");
			transforms.push_back({ SCALE, std::stof(fields[0], &sz) });
		}
		else if (this->currLine() == "<rotateX>")
		{
			this->loadElement(fields, "</rotateX>");
			transforms.push_back({ ROTATE_X, std::stof(fields[0], &sz) });
		}
		else if (this->currLine() == "<rotateY>")
		{
			this->loadElement(fields, "</rotateY>");
			transforms.push_back({ ROTATE_Y, std::stof(fields[0], &sz) });
		}
		else if (this->currLine() == "<rotateZ>")
		{
			this->loadElement(fields, "</rotateZ>");
			transforms.push_back({ ROTATE_Z, std::stof(fields[0], &sz) });
		}
		fields.clear();
	}
}

IBehavior* GameAssetLoader::loadBehavior()
{
	IBehavior* behavior = nullptr;
	//std::string::size_type sz;
	vector<string> fields;

	string behaviorType;
	std::vector<float> args;
	while (!this->atEnd() && this->nextLine() != "</behavior>")
	{
		if (this->currLine() == "<behaviorType>")
		{
			this->loadElement(fields, "</behaviorType>");
			behaviorType = fields[0];
		}
		else if (this->currLine() == "<args>")
		{
			this->loadElement(fields, "</args>");
			for (auto field : fields)
			{
				args.push_back(std::stof(field));
			}
		}
		fields.clear();
	}

	if (behaviorType == "BackForthBehavior")
	{
		behavior = new BackForthBehavior(args[0]);
	}
	else if (behaviorType == "RotateYBehavior")
	{
		behavior = new RotateYBehavior(args[0]);
	}

	return behavior;
}

void GameAssetLoader::loadBoundingBox(BoundingBox& bb)
{
	std::string::size_type sz;
	vector<string> fields;

	float height, width, depth;

	while (!this->atEnd() && this->nextLine() != "</boundingBox>")
	{
		if (this->currLine() == "<height>")
		{
			this->loadElement(fields, "</height>");
			height = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<width>")
		{
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		if (this->currLine() == "<depth>")
		{
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		fields.clear();
	}

	bb.set(width, depth, height);
}

void GameAssetLoader::loadLocalLightSources( char delimeter )
{
	LightSource* light;
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	std::string::size_type sz;
	float x, y, z, intensity;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</localLightSources>" )
	{
		this->split( fields, this->currLine(), delimeter );
		x = std::stof( fields[0], &sz );
		y = std::stof( fields[1], &sz );
		z = std::stof( fields[2], &sz );
		intensity = std::stof( fields[3], &sz );
		light = new LightSource();
		light->setPosition( x, y, z );
		light->setIntensity( intensity );
		graphics->getGameWorld()->localLights.push_back( light );
		fields.clear();
	}
}

void GameAssetLoader::loadObject( vector<string>& fields )
{
	Texture* texture = nullptr;
	OGLObject *object = nullptr;
	std::string::size_type sz;
	string name;
	if ( fields[0] == "<axis>" )
	{
		name = fields[1];
		float length = std::stof( fields[2], &sz );
		object = new Axis( name, length );
	}

	if ( object )
	{
		GameObjectManager* objectMgr =
			this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();
		objectMgr->addObject( name, object );
	}
}

void GameAssetLoader::loadTextures()
{
	string type, filepath, textureName;
	Texture* texture;
	vector<string> fields;
	while ( !this->atEnd() && this->nextLine() != "</textures>" )
	{
		fields.clear();
		this->split( fields, this->currLine(), ',' );
		type = fields[0];
		filepath = fields[1];
		textureName = fields[2];
		texture = this->loadTexture( type, filepath );
		if ( texture )
		{
			TextureManager* textureMgr =
				(TextureManager*)this->gameEngine->getGraphicsSystem()->getTextureManager();
			textureMgr->addTexture( textureName, texture );
		}
	}
}

Texture* GameAssetLoader::loadTexture( const string& type, const string& filepath )
{
	Texture* texture = nullptr;
	if ( type == "BMP" )
	{
		texture = this->loadBmpTexture( filepath );
	}
	return texture;
}

Texture* GameAssetLoader::loadBmpTexture( const string& filepath )
{
	BMPTextureLoader bmpLoader;
	bmpLoader.setFilePath( filepath );
	OGL2DTexture* texture = new OGL2DTexture( bmpLoader.load() );
	texture->setTexelFormat( GL_BGR );
	texture->setTypeOfData( GL_UNSIGNED_BYTE );
	texture->create();
	return texture;
}


