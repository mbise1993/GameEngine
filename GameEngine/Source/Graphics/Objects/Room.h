#pragma once
#ifndef ROOM
#define ROOM

#include <map>
#include "OGL3DCompositeObject.h"

class FlatSurface;
class Cuboid;

class Room : public OGL3DCompositeObject
{
private:
	FlatSurface* floor;
	FlatSurface* ceiling;
	Cuboid* frontWallTop;
	Cuboid* frontWallLeft;
	Cuboid* frontWallRight;
	Cuboid* backWall;
	Cuboid* leftWall;
	Cuboid* rightWall;
	std::map<std::string, OGLObject*> ownedObjects;

	float width, depth, height, doorHeight, doorWidth;

public:
	Room(const std::string& name, float width, float height, float depth, float doorHeight, float doorWidth,
		RGBA floorColor, RGBA ceilingColor, RGBA frontWallColor, RGBA backWallColor, RGBA leftWallColor, RGBA rightWallColor);
	virtual ~Room();

	void setShaderProgram(GLuint shaderProgram);

	void addOwnedObject(std::string& name, OGLObject* object);

	void update(glm::vec3 playerPosition, float elapsedSeconds);

	void render();

private:
	void applyObjectTransforms(const std::string& objectName, OGLReferenceFrame& referenceFrame);

	bool playerEnteredRoom(glm::vec3& playerPosition);
	bool playerExitedRoom(glm::vec3& playerPosition);
};

#endif