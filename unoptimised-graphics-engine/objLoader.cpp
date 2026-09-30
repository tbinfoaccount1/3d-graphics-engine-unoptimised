#include <fstream>
#include <string>
#include <SDL_image.h>

#include "objLoader.h"


Texture loadTexture(const std::string& filename)
{	
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (!surface)
	{
		throw std::runtime_error("Could not load texture: '" + filename + "': " + std::string(SDL_GetError()));
	}

	SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(surface);
	if (!convertedSurface)
	{
		throw std::runtime_error("Could not convert texture: '" + filename + "': " + std::string(SDL_GetError()));
	}

	Texture texture;
	texture.width = convertedSurface->w;
	texture.height = convertedSurface->h;
	texture.pixels.resize(texture.width * texture.height);
	
	std::memcpy(texture.pixels.data(), convertedSurface->pixels, texture.pixels.size() * sizeof(uint32_t));
	SDL_DestroySurface(convertedSurface);
	return texture;
}

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
		if (line.starts_with("v "))
		{
			float x, y, z;
			if (sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z) == 3)
			{
				objData.vertices.push_back(glm::vec3 {x, y, z});
			}
			else throw std::runtime_error("Unable to read vertex line: " + line);
		}
		else if (line.starts_with("vt "))
		{
			float x, y;
			if (sscanf_s(line.c_str(), "vt %f %f", &x, &y) == 2)
			{
				objData.textureCoords.push_back(glm::vec2{ x, y });
			}
			else throw std::runtime_error("Unable to read texture coordinate line: " + line);
		}
		else if (line.starts_with("vn "))
		{
			float x, y, z;
			if (sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3)
			{
				objData.normals.push_back(glm::vec3{ x, y, z });
			}
			else throw std::runtime_error("Unable to read normal line: " + line);
		}
		else if (line.starts_with("f "))
		{
			ObjFace face;
			int v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
			if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9)
			{
				face.vertexIndices.push_back(v1 - 1);
				face.vertexIndices.push_back(v2 - 1);
				face.vertexIndices.push_back(v3 - 1);
				face.textureCoordIndices.push_back(vt1 - 1);
				face.textureCoordIndices.push_back(vt2 - 1);
				face.textureCoordIndices.push_back(vt3 - 1);
				face.normalIndices.push_back(vn1 - 1);
				face.normalIndices.push_back(vn2 - 1);
				face.normalIndices.push_back(vn3 - 1);
				objData.faces.push_back(face);
			}
			else throw std::runtime_error("Unable to read face line: " + line);
		}
	}
	return objData;
}