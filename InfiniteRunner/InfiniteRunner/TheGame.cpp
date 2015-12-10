#include <Windows.h>
#include "TheGame.h"
#include "OGLShaderManager.h"
#include "GameEngine.h"
#include "OGLShaderManager.h"
#include "OGLGraphicsSystem.h"
#include "OGLViewingFrustum.h"
#include "GameWorld.h"
#include "LightSource.h"
#include "OGLCamera.h"
#include "CoreSystem.h"
#include "OGLVertexShader.h"
#include "TextFileReader.h"
#include "OGLFragmentShader.h"
#include "AssetLoader.h"
#include "OGLFirstPersonCamera.h"
#include "OGLFixedTargetCamera.h"
#include "OGLObject.h"
#include "RotateYBehavior.h"
#include "GameObjectManager.h"
#include "PCInputSystem.h"
#include "BackForthBehavior.h"
#include "OGL3DObject.h"
#include "OGL2DTextDisplay.h"
#include "BMPTextureLoader.h"
#include "OGL2DTexture.h"
#include "Plane.h"
#include "CustomTexture.h"
#include "TexturedCuboid.h"
#include "TextureManager.h"
#include "GameAssetLoader.h"
#include "LineBox.h"
#include "Sphere.h"
#include "OBJFileLoader.h"
#include "ScoreHud.h"
#include "HealthHud.h"
#include "ChunkSystem.h"
#include "PlayerController.h"
#include "Win32SoundManager.h"
#include "GameOverHUD.h"
#include "EnemyBehavior.h"

#include <gl\glew.h>
#include <glm\gtc\type_ptr.hpp>

using std::string;

TheGame::TheGame() : BaseObject( nullptr )
{}

TheGame::~TheGame()
{}

void TheGame::loadShaders( const string& shaderAssetFilename )
{
	GameAssetLoader* loader = (GameAssetLoader*)this->gameEngine->getAssetLoader();
	loader->loadAssetFile( shaderAssetFilename );
	loader->loadOnlyTheShaders();
}

void TheGame::setupGraphicsParameters( const string& uniformAssetFilename )
{
	GameAssetLoader* loader = (GameAssetLoader*)this->gameEngine->getAssetLoader();
	loader->loadAssetFile( uniformAssetFilename );
	loader->loadOnlyTheUniforms();
}

void TheGame::setupViewingEnvironment()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLViewingFrustum* frustum = (OGLViewingFrustum*)graphics->getViewingFrustum();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram* shader =
		(OGLShaderProgram*)shaderMgr->getShader( "ShaderProgramIllumination" );
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr( frustum->getMatrix() ) );
	shader->sendData( "cameraToScreenMatrix" );

	shader = (OGLShaderProgram*)shaderMgr->getShader( "ShaderProgram3d" );
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr( frustum->getMatrix() ) );
	shader->sendData( "cameraToScreenMatrix" );

	shader = (OGLShaderProgram*)shaderMgr->getShader( "TextureShader" );
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr( frustum->getMatrix() ) );
	shader->sendData( "cameraToScreenMatrix" );

	shader = (OGLShaderProgram*)shaderMgr->getShader( "SkyboxShader" );
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr( frustum->getMatrix() ) );
	shader->sendData( "cameraToScreenMatrix" );

	// Cull back faces and use counter-clockwise winding of front faces
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );

	// Enable depth testing
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LEQUAL );
	glDepthRange( 0.0f, 1.0f );
}

void TheGame::sendShaderData()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLViewingFrustum* frustum = (OGLViewingFrustum*)graphics->getViewingFrustum();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram* shader =
		(OGLShaderProgram*)shaderMgr->getShader( "ShaderProgramIllumination" );

	glm::vec3 globalLightPos = glm::vec3( 100, 100, 0 );
	shader->setUniformData(
		"worldGlobalLightPos",
		(void*)glm::value_ptr( globalLightPos ) );
	shader->sendData( "worldGlobalLightPos" );

	float globalLightIntensity = 0.1f;
	shader->setUniformData(
		"globalLightIntensity",
		(void*)&globalLightIntensity );
	shader->sendData( "globalLightIntensity" );

	GameWorld* gameWorld = graphics->getGameWorld();

	int numberOfLights = (int)gameWorld->localLights.size();
	shader->setUniformData(
		"numberOfLights",
		(void*)&numberOfLights );
	shader->sendData( "numberOfLights" );

	Vector3f lightPositions[10];
	float lightIntensities[10];
	for ( int i = 0; i < numberOfLights; i++ )
	{
		lightPositions[i] = gameWorld->localLights[i]->getPosition();
		lightIntensities[i] = gameWorld->localLights[i]->getIntensity();
	}
	shader->setUniformData(
		"lightPositions",
		(void*)lightPositions );
	shader->sendData( "lightPositions" );

	shader->setUniformData(
		"lightIntensities",
		(void*)lightIntensities );
	shader->sendData( "lightIntensities" );

	OGLCamera *camera = (OGLCamera*)gameWorld->getCamera();

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr( camera->orientation ) );
	shader->sendData( "worldToCameraMatrix" );

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr( glm::vec3( camera->orientation[3] ) ) );
	shader->sendData( "worldCameraPos" );

	// Texture Shader
	shader = (OGLShaderProgram*)shaderMgr->getShader( "TextureShader" );

	shader->setUniformData(
		"worldGlobalLightPos",
		(void*)glm::value_ptr( globalLightPos ) );
	shader->sendData( "worldGlobalLightPos" );

	shader->setUniformData(
		"globalLightIntensity",
		(void*)&globalLightIntensity );
	shader->sendData( "globalLightIntensity" );

	shader->setUniformData(
		"numberOfLights",
		(void*)&numberOfLights );
	shader->sendData( "numberOfLights" );

	shader->setUniformData(
		"lightPositions",
		(void*)lightPositions );
	shader->sendData( "lightPositions" );

	shader->setUniformData(
		"lightIntensities",
		(void*)lightIntensities );
	shader->sendData( "lightIntensities" );

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr( camera->orientation ) );
	shader->sendData( "worldToCameraMatrix" );

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr( glm::vec3( camera->orientation[3] ) ) );
	shader->sendData( "worldCameraPos" );

	// Basic 3D shader
	shader = (OGLShaderProgram*)shaderMgr->getShader( "ShaderProgram3d" );

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr( camera->orientation ) );
	shader->sendData( "worldToCameraMatrix" );

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr( glm::vec3( camera->orientation[3] ) ) );
	shader->sendData( "worldCameraPos" );

	// Skybox Shader
	shader = (OGLShaderProgram*)shaderMgr->getShader( "SkyboxShader" );

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr( camera->orientation ) );
	shader->sendData( "worldToCameraMatrix" );

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr( glm::vec3( camera->orientation[3] ) ) );
	shader->sendData( "worldCameraPos" );
}

void TheGame::setup( const string& gameAssetFilename )
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	TextureManager* texMgr = graphics->getTextureManager();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();

	OGLShaderProgram* plainShader3d = (OGLShaderProgram*)shaderMgr->getShader( "ShaderProgram3d" );
	OGLShaderProgram* textShader = (OGLShaderProgram*)shaderMgr->getShader( "TextShader" );
	OGLShaderProgram* skyboxShader = (OGLShaderProgram*)shaderMgr->getShader( "SkyboxShader" );
	OGLShaderProgram * shader2d = (OGLShaderProgram*)shaderMgr->getShader( "ShaderProgram2d" );

	AssetLoader* loader = this->gameEngine->getAssetLoader();
	loader->loadAssetFile( gameAssetFilename );
	loader->loadAssets();

	//
	// Setup camera
	//
	OGLFixedTargetCamera* camera = (OGLFixedTargetCamera*)graphics->getGameWorld()->getCamera();
	camera->setPosition( 5.0f, 1.5f, -10.0f, 180.0f );

	//
	// Setup score HUD
	//
	OGL2DTexture* texture = (OGL2DTexture*)texMgr->getTexture( "Letters" );
	OGLHudObject* scoreHud = new ScoreHud( 0.3f, 0.2f, -0.95f, -0.75f, { 0, 0, 1, 0.5f }, 0 );
	scoreHud->setShaderProgram( shader2d->getHandle() );
	scoreHud->setTextTexture( texture );
	scoreHud->setTextShaderProgram( textShader->getHandle() );
	graphics->addHudObject( scoreHud );
	//
	// Setup health HUD
	//
	OGLHudObject* healthHud = new HealthHud( 0.3f, 0.2f, 0.65f, -0.75f, { 1, 0, 0, 0.5f }, 100 );
	healthHud->setShaderProgram( shader2d->getHandle() );
	healthHud->setTextTexture( texture );
	healthHud->setTextShaderProgram( textShader->getHandle() );
	graphics->addHudObject( healthHud );
	//
	// Setup game over HUD
	//
	OGLHudObject* gameOverHud = new GameOverHUD( 0.6f, 0.3f, -0.3f, 0.15f, { 0, 1, 0, 1.0f } );
	gameOverHud->setShaderProgram( shader2d->getHandle() );
	gameOverHud->setTextTexture( texture );
	gameOverHud->setTextShaderProgram( textShader->getHandle() );
	graphics->addHudObject( gameOverHud );

	// Setup the player
	OGL3DGroupObject* playerObject = (OGL3DGroupObject*)graphics->getGameObject( "Player" );
	playerObject->setVisibility( true );

	// Setup events
	playerObject->registerSubscriber( SCORE_CHANGED_EVENT, scoreHud );
	playerObject->registerSubscriber( HEALTH_CHANGED_EVENT, healthHud );
	playerObject->registerSubscriber( GAME_OVER_EVENT, gameOverHud );
	playerObject->registerSubscriber( GAME_RESET_EVENT, gameOverHud );

	// Set as the player object // TODO: These values should be moved into the asset or config file
	objectMgr->setThePlayer( new PlayerController( playerObject, 100.0f, 10.0f, 0.5f, 20.0f ) );

	// Set the starting position
	objectMgr->getThePlayer()->setStartingPosition( glm::vec3( 0.0f, 0.0f, 25.0f ) );

	// Set the camera to follow the player
	camera->setTargetObject( playerObject );

	// Setup player controller events
	playerObject->registerSubscriber( GAME_OVER_EVENT, objectMgr->getThePlayer() );
	playerObject->registerSubscriber( GAME_RESET_EVENT, objectMgr->getThePlayer() );

	// Setup the Amo
	OGL3DGroupObject* amo = (OGL3DGroupObject*)graphics->getGameObject( "Amo" );
	amo->setVisibility( false );

	// Setup the enemy
	OGL3DGroupObject* depthCharge = (OGL3DGroupObject*)graphics->getGameObject( "DepthCharge" );
	depthCharge->setVisibility( true );
	depthCharge->setBehavior( new EnemyBehavior( objectMgr->getThePlayer(), amo ) );
	depthCharge->referenceFrame.setPosition( 30.0f, 0.0f, 0.0f  );

	// Play the music and start the game!!
	Win32SoundManager::playSound( Win32SoundManager::MUSIC );
}

void TheGame::processInputs()
{
	GameObjectManager* objectMgr = this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();

	// Get the player
	PlayerController* player = objectMgr->getThePlayer();

	// Process player inputs
	player->processInputs( this->gameEngine->getInputSystem() );
}
