#include "tangent.h"

void calculateTangents(ObjData& objData)
{
    for (int i = 0; i < objData.faces.size(); ++i)
    {
        ObjFace& face = objData.faces[i];

        glm::vec3 v0 = objData.vertices[face.vertexIndices[0]];
        glm::vec3 v1 = objData.vertices[face.vertexIndices[1]];
        glm::vec3 v2 = objData.vertices[face.vertexIndices[2]];

        glm::vec2 uv0 = objData.textureCoords[face.textureCoordIndices[0]];
        glm::vec2 uv1 = objData.textureCoords[face.textureCoordIndices[1]];
        glm::vec2 uv2 = objData.textureCoords[face.textureCoordIndices[2]];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float determinant = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
        if (std::abs(determinant) < 0.000001f)
            continue;

        float invDet = 1.0f / determinant;
        glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * invDet;
        glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * invDet;

        for (int faceIndex = 0; faceIndex < objData.faces.size(); ++faceIndex)
        {
            ObjFace& faceToCheck = objData.faces[faceIndex];
            for (int vertexIndexIndex = 0; vertexIndexIndex < 3; ++vertexIndexIndex)
            {
                for (int indexToCheck = 0; indexToCheck < 3; ++indexToCheck)
                {
                    if (face.vertexIndices[vertexIndexIndex] == faceToCheck.vertexIndices[indexToCheck] &&
                        face.normalIndices[vertexIndexIndex] == faceToCheck.normalIndices[indexToCheck] &&
                        face.textureCoordIndices[vertexIndexIndex] == faceToCheck.textureCoordIndices[indexToCheck])
                    {
                        faceToCheck.tangents[indexToCheck] += tangent;
                        faceToCheck.bitangents[indexToCheck] += bitangent;
                    }
                }
            }
        }
    }

    for (int i = 0; i < objData.faces.size(); ++i)
    {
        ObjFace& face = objData.faces[i];
        for (int j = 0; j < 3; ++j)
        {
            glm::vec3 N = glm::normalize(objData.normals[face.normalIndices[j]]);

            face.tangents[j] -= N * glm::dot(N, face.tangents[j]);

            float tangentLength = glm::length(face.tangents[j]);

            if (tangentLength > 0.000001f)
                face.tangents[j] /= tangentLength;

            glm::vec3 B = glm::cross(N, face.tangents[j]);

            if (glm::dot(B, face.bitangents[j]) < 0.0f)
                B = -B;

            face.bitangents[j] = B;
        }
    }
}