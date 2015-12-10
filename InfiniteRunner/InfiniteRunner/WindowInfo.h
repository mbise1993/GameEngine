#pragma once
#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

#include <string>
#include "RGBA.h"

typedef unsigned int uint;

//
// Encapsulates the information needed to create a new window.
// 
struct WindowInfo
{
public:
	std::wstring title;
	int startX;
	int startY;
	int width;
	int height;
	RGBA backgroundColor;
	WindowInfo() {}
};

#endif
