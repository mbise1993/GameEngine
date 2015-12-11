#include "OGLReferenceFrameStack.h"

OGLReferenceFrameStack::OGLReferenceFrameStack()
{
}

OGLReferenceFrameStack::~OGLReferenceFrameStack()
{
}

OGLReferenceFrame OGLReferenceFrameStack::pop()
{
	this->currFrame = this->frames.top();
	this->frames.pop();
	return this->currFrame;
}

void OGLReferenceFrameStack::push()
{
	this->frames.push(currFrame);
}
