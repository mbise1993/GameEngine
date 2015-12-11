#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glutilD.lib")

#include "WindowConfigReader.h"
#include "WindowInfo.h"
#include "GameEngine.h"
#include "WindowsConsoleLogger.h"
#include "GameWindow.h"
#include "Shaders/OGLShaderManager.h"
#include "TextFileReader.h"
#include "OGLGraphicsSystem.h"
#include "CoreSystem.h"
#include "GameWorld.h"
#include "Objects/GameObjectManager.h"
#include "Cameras/OGLFirstPersonCamera.h"
#include "OGLViewingFrustum.h"
#include "Objects/OGLObjectLoader.h"
#include "PCInputSystem.h"
#include "WindowsTimer.h"
#include "TheGame.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WindowConfigReader* configReader = new WindowConfigReader("../../Resources/Configuration/window.config");
	WindowInfo windowInfo;
	if(!configReader->isOpen())
	{
		return 1;
	}

	configReader->readConfigFile(windowInfo);

	GameEngine gameEngine(
		new WindowsConsoleLogger(),
		new TheGame(),
		new CoreSystem(
			new TextFileReader()), 
		new OGLGraphicsSystem(
			new OGLShaderManager(), 
			new GameWorld(
				new GameObjectManager(),
				new OGLFirstPersonCamera()),
			new OGLViewingFrustum()),
		new GameWindow(windowInfo),
		new PCInputSystem(),
		new WindowsTimer()
		);

	gameEngine.loadShaders();
	gameEngine.initializeWindowAndGraphics();
	OGLObjectLoader loader;
	gameEngine.setupGame(&loader);
	gameEngine.run();

	return 0;
}
