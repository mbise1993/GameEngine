#pragma once
#ifndef GAME_OBJECT_MANAGER
#define GAME_OBJECT_MANAGER

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <string>
using std::map;
using std::string;

class GameObject;
class LightSource;

class GameObjectManager
{
protected:
	map<string, GameObject*> allObjects;
	map<string, GameObject*> visibleObjects;
	std::vector<LightSource*> lightSources;

public:
	GameObjectManager();

	virtual ~GameObjectManager();

	void addObject(const string& name, GameObject* object);
	void addLightSource(LightSource* lightSource) { this->lightSources.push_back(lightSource); }

	map<string, GameObject*>& getVisibleObjects() { return this->visibleObjects; }
	std::vector<LightSource*>& getLightSources() { return this->lightSources; }
	map<string, GameObject*>& getAllObjects() { return this->allObjects; }

	GameObject* getObject(const string& name);

	void updateVisibleObjects();

	virtual void update(glm::vec3 playerPosition, float elapsedSeconds);

	virtual void renderVisibleObjects();
};

#endif

