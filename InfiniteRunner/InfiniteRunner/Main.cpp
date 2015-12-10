#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glutilD.lib")

#include "GameEngine.h"
#include "WindowsConsoleLogger.h"
#include "GameWindow.h"
#include "OGLVertexShader.h"
#include "OGLFragmentShader.h"
#include "OGLShaderProgram.h"
#include "OGLShaderManager.h"
#include "TextFileReader.h"
#include "OGLGraphicsSystem.h"
#include "CoreSystem.h"
#include "GameWorld.h"
#include "GameObjectManager.h"
#include "OGLFixedTargetCamera.h"
#include "OGLViewingFrustum.h"
#include "PCInputSystem.h"
#include "WindowsTimer.h"
#include "OGLFirstPersonCamera.h"
#include "TheGame.h"
#include "TextureManager.h"
#include "GameAssetLoader.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// Create the logger
	WindowsConsoleLogger *logger = new WindowsConsoleLogger();

	GameEngine gameEngine(
		logger,
		new TheGame(),
		new GameAssetLoader(),
		new CoreSystem(
			new TextFileReader() ),
		new OGLGraphicsSystem(
			new OGLShaderManager(),
			new GameWorld(
				new GameObjectManager(),
				new OGLFixedTargetCamera(),
				logger ),
			new OGLViewingFrustum(),
			new TextureManager() ),
		new GameWindow(
			L"The Game Window",
			1000,
			800 ),
		new PCInputSystem(),
		new WindowsTimer()
		);
	gameEngine.loadShaders( "ShaderAssets.data" );
	gameEngine.initializeWindowAndGraphics( "ShaderAssets.data" );
	gameEngine.setupGame( "GameAssets.data" );
	gameEngine.run();

	return 0;
}
