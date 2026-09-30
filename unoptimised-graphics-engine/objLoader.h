#pragma once
#include<string>
#include<vector>
#include<glm/glm.hpp>

struct Texture 
{
	int width{ 0 };
	int height{ 0 };

	std::vector<uint32_t> pixels;
};

struct ObjFace
{
	std::vector<int> vertexIndices;
	std::vector<int> textureCoordIndices;
	std::vector<int> normalIndices;
};


struct ObjData
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;

	std::vector<ObjFace> faces;


};

Texture loadTexture(const std::string& filename);

ObjData loadObjFile(const std::string& filename);