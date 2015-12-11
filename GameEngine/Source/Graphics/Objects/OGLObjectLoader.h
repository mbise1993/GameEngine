#pragma once
#ifndef OGL_OBJECT_LOADER
#define OGL_OBJECT_LOADER

#include <string>
#include "RGBA.h"
#include "Objects/ObjectLoader.h"

class OGLObject;

class OGLObjectLoader : public ObjectLoader
{
	public:
		OGLObjectLoader();
		~OGLObjectLoader();

		void loadObjects(GameObjectManager* gameObjectManager);
};

#endif