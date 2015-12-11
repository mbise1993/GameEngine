#pragma once
#ifndef OBJECT_LOADER
#define OBJECT_LOADER

class GameObjectManager;

class ObjectLoader
{
public:
	ObjectLoader();
	virtual ~ObjectLoader();

	virtual void loadObjects(GameObjectManager *gameObjectManager) = 0;
};

#endif

