#pragma once
#ifndef OGL_OBJECT_READER
#define OGL_OBJECT_READER

#include <string>
#include <vector>
#include <map>
#include "Objects/OGL3DObject.h"
#include "RGBA.h"
#include "Vertex.h"
#include "Material.h"

struct XYZ
{
	float x;
	float y;
	float z;
};

struct UV
{
	float u;
	float v;
};

struct Face
{
	short position1, texture1, normal1;
	short position2, texture2, normal2;
	short position3, texture3, normal3;
};

//
// Reads in .obj (WaveFront) and .mtl (material) files and uses the data to create GameObject
//
class OGLObjectReader
{
	private:
		std::vector<Vertex>* allVertexData;
		std::map<std::string, VBOObject*> vboObjects;
		std::string objFileContents;
		std::string mtlFileContents;

		bool parseObjFileContents();
		bool readGroupName(std::string data, std::string& groupName);
		bool readPosition(std::string data, XYZ& position);
		bool readUV(std::string data, UV& uv);
		bool readNormal(std::string data, XYZ& normal);
		bool readFace(std::string data, Face& face);
		bool readMaterialName(std::string data, std::string& materialName);
		bool createVBOObject(std::string& groupName, std::map<int, XYZ>& positions, std::map<int, UV>& uvs,
			std::map<int, XYZ>& normals, std::vector<Face>& faces, std::string& materialName);
		bool readMaterial(const std::string materialName, Material& material);
		bool readDiffuseColor(std::string data, RGBA& diffuseColor);
		bool readSpecularColor(std::string data, RGBA& specularColor);
		bool readDissolveFactor(std::string data, float& dissolveFactor);
		bool readShininess(std::string data, float& shininess);

	public:
		OGLObjectReader();
		~OGLObjectReader();
		
		bool readFile(std::string filePath);
		std::map<std::string, VBOObject*> getVBOObjects() { return this->vboObjects; }
		OGLObject* getOGLObject(const std::string name);
};

#endif