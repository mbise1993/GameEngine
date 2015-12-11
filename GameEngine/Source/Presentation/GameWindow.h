#pragma once
#ifndef GAME_WINDOW
#define GAME_WINDOW

#include "WindowInfo.h"
#include "Win32OGLWindow.h"

class Renderer;

class GameWindow :
	public Win32OGLWindow
{
private:
	static GameWindow *self;

public:
	GameWindow(WindowInfo& windowInfo);

	virtual ~GameWindow();

	bool createRenderer();

protected:
	void runOneFrame(float elapsedSeconds);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void updateWindowSize(int width, int height);
};

#endif

