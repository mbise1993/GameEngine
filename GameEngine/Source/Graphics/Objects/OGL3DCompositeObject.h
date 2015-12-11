#pragma once
#ifndef OGL_3D_COMPOSITE_OBJECT
#define OGL_3D_COMPOSITE_OBJECT

#include "OGL3DObject.h"
#include "OGLReferenceFrameStack.h"

class OGL3DCompositeObject : public OGL3DObject
{
public:
	// A reference frame stack is used to manage the hierarchy
	OGLReferenceFrameStack frameStack;

public:
	OGL3DCompositeObject(const string& name);
	virtual ~OGL3DCompositeObject();
};

#endif

