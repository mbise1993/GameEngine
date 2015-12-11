#include "OGLObjectReader.h"
#include "ElementArray.h"
#include "TextFileReader.h"
#include "BMPImageReader.h"
#include <fstream>
#include <sstream>
#include <boost\lexical_cast.hpp>

//
// Default constructor
//
OGLObjectReader::OGLObjectReader()
{
	this->allVertexData = new std::vector<Vertex>();
}

//
// Destructor
//
OGLObjectReader::~OGLObjectReader()
{
	delete this->allVertexData;
}

bool OGLObjectReader::readFile(std::string filePath)
{
	if (filePath.empty())
		return false;

	// Clear fields
	this->objFileContents.clear();
	this->mtlFileContents.clear();
	this->allVertexData->clear();
	this->vboObjects.clear();

	TextFileReader* fileReader = new TextFileReader();

	// Get contents of .obj file
	this->objFileContents = fileReader->readContents(filePath);
	if (this->objFileContents.empty())
		return false;

	// Find .mtl filename in .obj file and get contents of .mtl file. If not line beginning
	// with 'mtllib' is found after reading 10 lines, assume there is not .mtl file.
	std::stringstream objFileStream(this->objFileContents);
	std::string currentLine;
	std::string type;
	int counter = 10;
	while (counter > 0 && getline(objFileStream, currentLine, '\n'))
	{
		if (currentLine.empty())
			continue;

		std::stringstream lineParser(currentLine);
		lineParser >> type;

		if (type == "mtllib")
		{
			std::string mtlFileName;
			lineParser >> mtlFileName;
			this->mtlFileContents = fileReader->readContents("../../Resources/Objects/" + mtlFileName);
			break;
		}

		counter--;
	}

	return parseObjFileContents();
}

//
// Uses VBOObjects to construct and return an OGLObject
//
OGLObject* OGLObjectReader::getOGLObject(const std::string name)
{
	OGLObject* oglObject = new OGL3DObject(name);
	oglObject->setArrayBufferType();
	
	for (auto it = this->vboObjects.begin(); it != this->vboObjects.end(); it++)
		oglObject->addVBOObject(it->second);

	return oglObject;
}

//
// Reads object data from .obj file
//
bool OGLObjectReader::parseObjFileContents()
{
	std::string groupName;
	int positionIndexer = 1;
	std::map<int, XYZ> positions;
	int uvIndexer = 1;
	std::map<int, UV> uvs;
	int normalIndexer = 1;
	std::map<int, XYZ> normals;
	std::vector<Face> faces;
	std::string materialName;

	std::stringstream fileContentsStream(this->objFileContents);
	std::string currentLine;
	std::string type;
	while (std::getline(fileContentsStream, currentLine, '\n'))
	{
		if (currentLine.empty())
			continue;

		std::stringstream parser(currentLine);
		parser >> type;

		if (type == "g")
		{
			// If this is not the first group, then create VBOObject with data
			if(!positions.empty() && !normals.empty() && !faces.empty())
			{
				if (!createVBOObject(groupName, positions, uvs, normals, faces, materialName))
					return false;
			}

			if (!readGroupName(currentLine, groupName))
				return false;
		}
		if (type == "v")
		{
			XYZ position;
			if (!readPosition(currentLine, position))
				return false;

			positions[positionIndexer++] = position;
		}
		if (type == "vt")
		{
			UV uv;
			if (!readUV(currentLine, uv))
				return false;

			uvs[uvIndexer++] = uv;
		}
		if (type == "vn")
		{
			XYZ normal;
			if (!readNormal(currentLine, normal))
				return false;

			normals[normalIndexer++] = normal;
		}
		if (type == "f")
		{
			Face face;
			if (!readFace(currentLine, face))
				return false;

			faces.push_back(face);
		}
		if (type == "usemtl")
		{
			if (!readMaterialName(currentLine, materialName))
				return false;
		}
	}

	// When EOF reached, create VBOObject for last group
	if(!positions.empty() && !normals.empty() && !faces.empty())
	{
		if (!createVBOObject(groupName, positions, uvs, normals, faces, materialName))
			return false;
	}

	return true;
}

//
// Reads the object name from the line
//
bool OGLObjectReader::readGroupName(std::string data, std::string& groupName)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'g' and discard first character
	parser >> temp;
	if (temp != "g")
		return false;

	while (parser >> temp)
	{
		groupName += temp;
		groupName += " ";
	}

	groupName = groupName.substr(0, groupName.size() - 1);	// Discard trailing space

	return true;
}

//
// Reads a vertex from the current line
//
bool OGLObjectReader::readPosition(std::string data, XYZ& position)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'v' and discard first character
	parser >> temp;	
	if (temp != "v")
		return false;

	// Read X value
	parser >> temp;
	position.x = boost::lexical_cast<float>(temp);
	// Read Y value
	parser >> temp;
	position.y = boost::lexical_cast<float>(temp);
	// Read Z value
	parser >> temp;
	position.z = boost::lexical_cast<float>(temp);

	return true;
}

//
// Reads a vertex's UV data from the current line
//
bool OGLObjectReader::readUV(std::string data, UV& uv)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'vt' and discard it
	parser >> temp;
	if (temp != "vt")
		return false;

	// Read U value
	parser >> temp;
	uv.u = boost::lexical_cast<float>(temp);
	// Read Y value
	parser >> temp;
	uv.v = boost::lexical_cast<float>(temp);

	return true;
}

//
// Reads a vertexs normal data from the current line
//
bool OGLObjectReader::readNormal(std::string data, XYZ& normal)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'vn' and discard it
	parser >> temp;
	if (temp != "vn")
		return false;

	// Read X value
	parser >> temp;
	normal.x = boost::lexical_cast<float>(temp);
	// Read Y value
	parser >> temp;
	normal.y = boost::lexical_cast<float>(temp);
	// Read Z value
	parser >> temp;
	normal.z = boost::lexical_cast<float>(temp);

	return true;
}

//
// Reads a face from the current line
//
bool OGLObjectReader::readFace(std::string data, Face& face)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'f' and discard first character
	parser >> temp;
	if (temp != "f")
		return false;

	short slashPos;
	std::string token;

	// Get values for first vertex in face
	parser >> temp;
	// Position
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	face.position1 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Texture
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	//face.texture1 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Normal
	face.normal1 = boost::lexical_cast<short>(temp);

	// Get values for second vertex in face
	parser >> temp;
	// Position
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	face.position2 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Texture
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	//face.texture2 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Normal
	face.normal2 = boost::lexical_cast<short>(temp);

	// Get values for third vertex in face
	parser >> temp;
	// Position
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	face.position3 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Texture
	slashPos = temp.find('/', 0);
	token = temp.substr(0, slashPos);
	//face.texture3 = boost::lexical_cast<short>(token);
	temp.erase(0, slashPos + 1);
	// Normal
	face.normal3 = boost::lexical_cast<short>(temp);

	return true;
}

//
// Reads the material name from the line
//
bool OGLObjectReader::readMaterialName(std::string data, std::string& materialName)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'usemtl' and discard first character
	parser >> temp;
	if (temp != "usemtl")
		return false;

	while (parser >> temp)
	{
		materialName += temp;
		materialName += " ";
	}

	materialName = materialName.substr(0, materialName.size() - 1);	// Discard trailing space

	return true;
}

bool OGLObjectReader::createVBOObject(std::string& groupName, std::map<int, XYZ>& positions, std::map<int, UV>& uvs,
	std::map<int, XYZ>& normals, std::vector<Face>& faces, std::string& materialName)
{
	Material material;
	if (!readMaterial(materialName, material))
		return false;

	material.ambientIntensity = 0.1f;
	material.specularColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	std::vector<Vertex>* vertices = new std::vector<Vertex>();
	XYZ component;
	for (Face face : faces)
	{
		// Create first vertex in face
		Vertex vertex1;
		// Set position data
		component = positions[face.position1];
		vertex1.positionX = component.x;
		vertex1.positionY = component.y;
		vertex1.positionZ = component.z;
		vertex1.positionW = 1.0f;
		// Set normal data
		component = normals[face.normal1];
		vertex1.normalX = component.x;
		vertex1.normalY = component.y;
		vertex1.normalZ = component.z;
		// Set color data
		vertex1.red = material.diffuseColor.red;
		vertex1.green = material.diffuseColor.green;
		vertex1.blue = material.diffuseColor.blue;
		vertex1.alpha = material.diffuseColor.alpha;

		// Create second vertex in face
		Vertex vertex2;
		// Set position data
		component = positions[face.position2];
		vertex2.positionX = component.x;
		vertex2.positionY = component.y;
		vertex2.positionZ = component.z;
		vertex2.positionW = 1.0f;
		// Set normal data
		component = normals[face.normal2];
		vertex2.normalX = component.x;
		vertex2.normalY = component.y;
		vertex2.normalZ = component.z;
		// Set color data
		vertex2.red = material.diffuseColor.red;
		vertex2.green = material.diffuseColor.green;
		vertex2.blue = material.diffuseColor.blue;
		vertex2.alpha = material.diffuseColor.alpha;

		// Create third vertex in face
		Vertex vertex3;
		// Set position data
		component = positions[face.position3];
		vertex3.positionX = component.x;
		vertex3.positionY = component.y;
		vertex3.positionZ = component.z;
		vertex3.positionW = 1.0f;
		// Set normal data
		component = normals[face.normal3];
		vertex3.normalX = component.x;
		vertex3.normalY = component.y;
		vertex3.normalZ = component.z;
		// Set color data
		vertex3.red = material.diffuseColor.red;
		vertex3.green = material.diffuseColor.green;
		vertex3.blue = material.diffuseColor.blue;
		vertex3.alpha = material.diffuseColor.alpha;

		// Add to vertices
		vertices->push_back(vertex1);
		vertices->push_back(vertex2);
		vertices->push_back(vertex3);
	}

	// Create new VBOObject using data and add it to vector
	VBOObject* vboObject = OGLObject::createVBOObject(groupName, vertices, GL_TRIANGLES, material);
	this->vboObjects[groupName] = vboObject;
	this->allVertexData->insert(this->allVertexData->end(), vertices->begin(), vertices->end());

	// Clear parameters for next group
	groupName.clear();
	positions.clear();
	uvs.clear();
	normals.clear();
	faces.clear();
	materialName.clear();

	return true;
}

bool OGLObjectReader::readMaterial(const std::string materialName, Material& material)
{
	std::stringstream mtlFileStream(this->mtlFileContents);
	std::string currentLine;
	std::string type;

	// First, find the correct material definition
	while (getline(mtlFileStream, currentLine, '\n'))
	{
		if (currentLine.empty())
			continue;

		std::stringstream lineParser(currentLine);
		lineParser >> type;

		// If new material definition found, check name
		if (type == "newmtl")
		{
			lineParser >> type;

			// If correct material found, exit loop with parser set to correct position
			if (type == materialName)
			{
				break;
			}
		}
	}

	// Then, read material data
	while(getline(mtlFileStream, currentLine, '\n'))
	{
		if (currentLine.empty())
			continue;

		std::stringstream lineParser(currentLine);
		lineParser >> type;

		// If we hit another material definition, exit
		if (type == "newmtl")
			break;

		if(type == "Kd")
		{
			if (!readDiffuseColor(currentLine, material.diffuseColor))
				return false;
		}
		if (type == "Ks")
		{
			if (!readSpecularColor(currentLine, material.specularColor))
				return false;
		}
		if (type == "d")
		{
			if (!readDissolveFactor(currentLine, material.diffuseColor.alpha))
				return false;

			material.specularColor.alpha = material.diffuseColor.alpha;
		}
		if (type == "Ns")
		{
			if (!readShininess(currentLine, material.specularExp))
				return false;
		}
		if (type == "map_Kd")
		{
			/*if (!readTextureFileName(currentLine))
				return false;*/
		}
	}
}

bool OGLObjectReader::readDiffuseColor(std::string data, RGBA& diffuseColor)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'Kd' and discard first character
	parser >> temp;
	if (temp != "Kd")
		return false;

	// Read red value
	parser >> temp;
	diffuseColor.red = boost::lexical_cast<float>(temp);
	// Read green value
	parser >> temp;
	diffuseColor.green = boost::lexical_cast<float>(temp);
	// Read blue value
	parser >> temp;
	diffuseColor.blue = boost::lexical_cast<float>(temp);

	return true;
}

bool OGLObjectReader::readSpecularColor(std::string data, RGBA& specularColor)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'Ks' and discard first character
	parser >> temp;
	if (temp != "Ks")
		return false;

	// Read red value
	parser >> temp;
	specularColor.red = boost::lexical_cast<float>(temp);
	// Read green value
	parser >> temp;
	specularColor.green = boost::lexical_cast<float>(temp);
	// Read blue value
	parser >> temp;
	specularColor.blue = boost::lexical_cast<float>(temp);

	return true;
}

bool OGLObjectReader::readDissolveFactor(std::string data, float& dissolveFactor)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'd' and discard first character
	parser >> temp;
	if (temp != "d")
		return false;

	// Read dissolve factor
	parser >> temp;
	dissolveFactor = boost::lexical_cast<float>(temp);

	return true;
}

bool OGLObjectReader::readShininess(std::string data, float& shininess)
{
	if (data.empty())
		return false;

	std::string temp;
	std::stringstream parser(data);

	// Check for 'Ns' and discard it
	parser >> temp;
	if (temp != "Ns")
		return false;

	// Read shininess
	parser >> temp;
	shininess = boost::lexical_cast<float>(temp) / 10000.0f;

	return true;
}