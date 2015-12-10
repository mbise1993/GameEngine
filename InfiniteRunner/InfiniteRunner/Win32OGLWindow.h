#pragma once
#ifndef WIN32_OGL_WINDOW
#define WIN32_OGL_WINDOW

#include "Win32Window.h"
#include "RGBA.h"

#include <string>

class Win32OGLWindow :
	public Win32Window
{
protected:
	HGLRC renderingContext;
	HDC deviceContext;
	static Win32OGLWindow *self;
	std::string oglVersion;
	RGBA background;

public:
	Win32OGLWindow( std::wstring title, int width, int height );
	virtual ~Win32OGLWindow();

	bool create();

	const std::string& getOpenGLVersionInformation() const { return this->oglVersion; }

protected:
	void runOneFrame( float elapsedSeconds );

	virtual void updateWindowSize( int width, int height );

	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};

#endif

