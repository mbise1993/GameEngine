#pragma once
#ifndef GAME_ASSET_LOADER
#define GAME_ASSET_LOADER

#include "AssetLoader.h"
#include "Skybox.h"
#include "ChunkSystem.h"

#include <map>
#include <string>
#include <vector>

class GameObjectManager;
class TextureManager;
class Texture;
class IBehavior;

enum TransformTypes
{
	SCALE, ROTATE_X, ROTATE_Y, ROTATE_Z
};

struct TransformDefinition
{
	TransformTypes type;
	float value;
};

class GameAssetLoader : public AssetLoader
{
public:
	GameAssetLoader();

	virtual ~GameAssetLoader();

	void loadAssets();

	void loadOnlyTheShaders();

	void loadOnlyTheUniforms();

protected:
	void loadShaders();

	void loadVertexShaders();

	void loadFragmentShaders();

	void loadShaderPrograms();

	void loadUniforms();

	void loadTextures();

	void loadGameObjects();

	void loadChunkSystemData();

	void loadChunkSettings(ChunkSettings* chunkSettings);

	void loadChunkObjects(ChunkGenerator* chunkGenerator);

	void loadLocalLightSources( char delimeter = ',' );

	Texture* loadTexture( const std::string& type, const std::string& filepath );

	Texture* loadBmpTexture( const std::string& filepath );

	void loadObject( std::vector<std::string>& fields );

	void loadSkybox();
	void loadSkyboxTextureNames( std::map<SkyboxSide, std::string>& texNames );

	void loadAxis();

	void loadOTFS();

	void loadTexturedCuboid();

	void loadSphere();

	void loadCuboid();

	void loadLineBox();

	OGL3DObject* loadObjFile();

	void loadBoundingBox(BoundingBox& bb);

	void loadTransforms(std::vector<TransformDefinition>& transforms);

	IBehavior* loadBehavior();
};

#endif

