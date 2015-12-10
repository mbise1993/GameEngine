#pragma once
#ifndef GAME_OBJECT_MANAGER
#define GAME_OBJECT_MANAGER

#include <map>
#include <string>

class GameObject;
class OGLObject;
class ChunkSystem;
class PlayerController;

class GameObjectManager
{
protected:
	std::map<std::string, GameObject*> *allObjects;
	std::map<std::string, GameObject*> *visibleObjects;
	ChunkSystem *chunkSystem;
	PlayerController *thePlayer;

public:
	GameObjectManager();

	virtual ~GameObjectManager();

	void addObject( const std::string& name, GameObject* object );

	void updateVisibleObjects();

	std::map<std::string, GameObject*> &getVisibleObjects()
	{
		return *this->visibleObjects;
	}

	std::map<std::string, GameObject*> &getAllObjects()
	{
		return *this->allObjects;
	}

	GameObject* getObject( const std::string& name );

	virtual void update( float elapsedSeconds );

	virtual void renderVisibleObjects();

	virtual void clearCollisions();

	virtual void checkForCollisions();

	virtual void setChunkSystem( ChunkSystem *chunkSystem ) { this->chunkSystem = chunkSystem; }

	virtual ChunkSystem* getChunkSystem() { return this->chunkSystem; }

	virtual void setThePlayer( PlayerController *player ) { this->thePlayer = player; }

	virtual PlayerController* getThePlayer() { return this->thePlayer; }
};

#endif

