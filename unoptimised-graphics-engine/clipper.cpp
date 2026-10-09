
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

float getBoundaryDistance(const glm::vec4& ClipVertex, ClipPlane plane)
{
	switch (plane)
	{
		case ClipPlane::Left:
			return ClipVertex.x + ClipVertex.w;

		case ClipPlane::Right:
			return ClipVertex.w - ClipVertex.x;

		case ClipPlane::Bottom:
			return ClipVertex.y + ClipVertex.w;

		case ClipPlane::Top:
			return ClipVertex.w - ClipVertex.y;

		case ClipPlane::Near:
			return ClipVertex.z + ClipVertex.w;

		case ClipPlane::Far:
			return ClipVertex.w - ClipVertex.z;
	}
	return 0.0f;
}

std::vector<std::array<ClipVertex, 3>> clipTriangle(const ClipVertex v0, const ClipVertex v1, const ClipVertex v2)
{
	std::vector<ClipVertex> polygon { v0, v1, v2 };

	for (ClipPlane plane : planes)
	{
		std::vector<ClipVertex> newPolygon;
		for (size_t i = 0; i < polygon.size(); ++i)
		{
			const ClipVertex& currentClipVertex = polygon[i];
			const ClipVertex& nextClipVertex = polygon[(i + 1) % polygon.size()];
			float currentDistance = getBoundaryDistance(currentClipVertex.position, plane);
			float nextDistance = getBoundaryDistance(nextClipVertex.position, plane);
			if (currentDistance >= 0)
			{
				newPolygon.push_back(currentClipVertex);
			}
			if ((currentDistance >= 0 && nextDistance < 0) || (currentDistance < 0 && nextDistance >= 0))
			{
				float t = currentDistance / (currentDistance - nextDistance);
				ClipVertex intersection;
				intersection.position = currentClipVertex.position + t * (nextClipVertex.position - currentClipVertex.position);
				intersection.uv = currentClipVertex.uv + t * (nextClipVertex.uv - currentClipVertex.uv);
				newPolygon.push_back(intersection);
			}
		}
		polygon = newPolygon;

		if (polygon.size() < 3)
		{
			return {};
		}
	}

	std::vector<std::array<ClipVertex, 3>> triangles;
	for (size_t i = 1; i < polygon.size() - 1; ++i)
	{
		std::array<ClipVertex, 3> triangle = { polygon[0], polygon[i], polygon[i + 1] };
		triangles.push_back(triangle);
	}
	return triangles;
}