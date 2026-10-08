
#include "clipper.h"

enum ClipPlane
{
	Left,
	Right,
	Bottom,
	Top,
	Near,
	Far
};

const ClipPlane planes[] = { ClipPlane::Left, ClipPlane::Right, ClipPlane::Bottom, ClipPlane::Top, ClipPlane::Near, ClipPlane::Far };

float getBoundaryDistance(const glm::vec4& vertex, ClipPlane plane)
{
	switch (plane)
	{
		case ClipPlane::Left:
			return vertex.x + vertex.w;

		case ClipPlane::Right:
			return vertex.w - vertex.x;

		case ClipPlane::Bottom:
			return vertex.y + vertex.w;

		case ClipPlane::Top:
			return vertex.w - vertex.y;

		case ClipPlane::Near:
			return vertex.z + vertex.w;

		case ClipPlane::Far:
			return vertex.w - vertex.z;
	}
	return 0.0f;
}

std::vector<std::array<glm::vec4, 3>> clipTriangle(const glm::vec4 v0, const glm::vec4 v1, const glm::vec4 v2)
{
	std::vector<glm::vec4> polygon { v0, v1, v2 };

	for (ClipPlane plane : planes)
	{
		std::vector<glm::vec4> newPolygon;
		for (size_t i = 0; i < polygon.size(); ++i)
		{
			const glm::vec4& currentVertex = polygon[i];
			const glm::vec4& nextVertex = polygon[(i + 1) % polygon.size()];
			float currentDistance = getBoundaryDistance(currentVertex, plane);
			float nextDistance = getBoundaryDistance(nextVertex, plane);
			if (currentDistance >= 0)
			{
				newPolygon.push_back(currentVertex);
			}
			if ((currentDistance >= 0 && nextDistance < 0) || (currentDistance < 0 && nextDistance >= 0))
			{
				float t = currentDistance / (currentDistance - nextDistance);
				glm::vec4 intersection = currentVertex + t * (nextVertex - currentVertex);
				newPolygon.push_back(intersection);
			}
		}
		polygon = newPolygon;

		if (polygon.size() < 3)
		{
			return {};
		}
	}

	std::vector<std::array<glm::vec4, 3>> triangles;
	for (size_t i = 1; i < polygon.size() - 1; ++i)
	{
		std::array<glm::vec4, 3> triangle = { polygon[0], polygon[i], polygon[i + 1] };
		triangles.push_back(triangle);
	}
	return triangles;
}