#include <fstream>
#include <string>
#include "objLoader.h"


ObjData loadObjFile(const std::string& filename)
{
	ObjData objData;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + filename);
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::string prefix{ line.substr(0, 2) };
		if (prefix == "v ")
		{
			float x, y, z;
			if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3)
			{
				objData.vertices.push_back(glm::vec3 {x, y, z});
			}
			else throw std::runtime_error("Unable to read vertex line: " + line);
		}
		else if (prefix == "vt")
		{
			float x, y;
			if (sscanf(line.c_str(), "vt %f %f", &x, &y) == 2)
			{
				objData.textureCoords.push_back(glm::vec2{ x, y });
			}
			else throw std::runtime_error("Unable to read texture coordinate line: " + line);
		}
		else if (prefix == "vn")
		{
			float x, y, z;
			if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3)
			{
				objData.normals.push_back(glm::vec3{ x, y, z });
			}
			else throw std::runtime_error("Unable to read normal line: " + line);
		}
		else if (prefix == "f ")
		{
			ObjFace face;
			int v1, v2, v3;
			if (sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3) == 3)
			{
				face.vertexIndices.push_back(v1 - 1);
				face.vertexIndices.push_back(v2 - 1);
				face.vertexIndices.push_back(v3 - 1);
				objData.faces.push_back(face);
			}
			else if (sscanf(line.c_str(), "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) == 3)
			{
				face.vertexIndices.push_back(v1 - 1);
				face.vertexIndices.push_back(v2 - 1);
				face.vertexIndices.push_back(v3 - 1);
				objData.faces.push_back(face);
			}
			else throw std::runtime_error("Unable to read face line: " + line);
		}
	}
	return objData;
}