#pragma once
#ifndef SCORE_CHANGED_EVENT_ARGS
#define SCORE_CHANGED_EVENT_ARGS

#include "EventArgs.h"

class ScoreChangedEventArgs : public EventArgs
{
private:
	int scoreDelta;

public:
	ScoreChangedEventArgs( int scoreDelta );
	virtual ~ScoreChangedEventArgs();

	int getScoreDelta() { return this->scoreDelta; }
};

#endif