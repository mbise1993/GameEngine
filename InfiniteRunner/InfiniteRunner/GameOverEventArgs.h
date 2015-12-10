#pragma once
#include "EventArgs.h"
class GameOverEventArgs :
	public EventArgs
{
protected:
	int score;
public:	
	GameOverEventArgs(int score) : EventArgs(GAME_OVER_EVENT),score(score) {	};
	virtual ~GameOverEventArgs() {	};

	int getScore() { return this->score; }
};

