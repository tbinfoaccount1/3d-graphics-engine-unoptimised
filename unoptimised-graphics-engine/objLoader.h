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

struct Material
{
	std::string name;

	glm::vec3 ambientColor{ 1.0f };
	glm::vec3 diffuseColor{ 1.0f };
	glm::vec3 specularColor{ 1.0f };
	glm::vec3 transmissionFilter{ 1.0f };

	float shininess{ 0.0f };
	float opticalDensity{ 1.0f };

	float dissolve{ 1.0f };

	Texture diffuseTexture;
	Texture specularTexture;

};

struct ObjFace
{
	std::vector<int> vertexIndices;
	std::vector<int> textureCoordIndices;
	std::vector<int> normalIndices;

	int materialIndex{ -1 };
};


struct ObjData
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;

	std::vector<ObjFace> faces;

	std::vector<Material> materials;
};

Texture loadTexture(const std::string& filename);

std::vector<Material> loadMtlFile(const std::string& filename);

ObjData loadObjFile(const std::string& filename);