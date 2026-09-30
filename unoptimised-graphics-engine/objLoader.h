#pragma once
#include<string>
#include<vector>
#include<glm/glm.hpp>

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

ObjData loadObjFile(const std::string& filename);