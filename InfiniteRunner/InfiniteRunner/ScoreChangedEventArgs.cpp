#include "ScoreChangedEventArgs.h"

ScoreChangedEventArgs::ScoreChangedEventArgs( int scoreDelta ) :
	EventArgs( SCORE_CHANGED_EVENT )
{
	this->scoreDelta = scoreDelta;
}

ScoreChangedEventArgs::~ScoreChangedEventArgs()
{}
