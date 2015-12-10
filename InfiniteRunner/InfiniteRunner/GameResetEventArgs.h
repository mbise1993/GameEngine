#pragma once
#include "EventArgs.h"
class GameResetEventArgs :
	public EventArgs
{
public:
	GameResetEventArgs() : EventArgs(GAME_RESET_EVENT) { };
	~GameResetEventArgs() { };
};

