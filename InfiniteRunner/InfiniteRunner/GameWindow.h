#pragma once
#ifndef GAME_WINDOW
#define GAME_WINDOW

#include "Win32OGLWindow.h"

class Renderer;

class GameWindow :
	public Win32OGLWindow
{
private:
	static GameWindow *self;

public:
	GameWindow( std::wstring title, int width, int height );

	virtual ~GameWindow();

protected:
	void runOneFrame( float elapsedSeconds );

	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void updateWindowSize( int width, int height );
};

#endif

