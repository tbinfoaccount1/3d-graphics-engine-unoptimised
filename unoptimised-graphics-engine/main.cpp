#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "objLoader.h"
#include "rasterizer.h"
#include "clipper.h"


// Schermgrootte
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

FrameBuffer frameBuffer(kScreenWidth, kScreenHeight);


// Camera
glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 cameraRotation(0.0f, -90.0f, 0.0f);
float cameraSensitivity{ 2.0f };

glm::vec3 cameraMovement(0.0f, 0.0f, 0.0f);
float cameraSpeed{ 0.3f };

// Object
glm::vec3 objectPosition(0.5f, 0.0f, 0.0f);
glm::vec3 objectRotation(105.0f, 25.0f, 60.0f);
glm::vec3 objectScale(1.0f, 2.0f, 1.5f);


// Functies initialiseren
bool init();
void close();



SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };



bool init()
{
    bool success{ true };

	// SDL initialiseren
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
		// Window aanmaken
        if (window = SDL_CreateWindow("3D Graphics Engine - unoptimised", kScreenWidth, kScreenHeight, 0); window == nullptr)
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            SDL_Log("Window created");
			renderer = SDL_CreateRenderer(window, nullptr);
			if (!renderer)
			{
				SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
				success = false;
			}
        }
    }

    

    return success;
}


void close()
{
    SDL_DestroyWindow(window);
    window = nullptr;

    // SDL afsluiten
    SDL_Quit();
}


int main(int argc, char* args[])
{
    int exitCode{ 0 };


    // init maakt een window aan met SDL en koppelt screenSurface aan de window
    if (!init())
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
        close();
		return exitCode;
    }

	SDL_Texture* frameBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, kScreenWidth, kScreenHeight);
	ObjData objData = loadObjFile("cube.obj");

    bool quit{ false };
    bool wPressed{ false }, aPressed{ false }, sPressed{ false }, dPressed{ false }, spacePressed{ false }, cPressed{ false }, ePressed{ false }, qPressed{ false };

    SDL_Event e;
    SDL_zero(e);

    while (quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP)
            {
	            bool isKeyDown = e.type == SDL_EVENT_KEY_DOWN;
                switch (e.key.key)
                {
		            case SDLK_W:
			            wPressed = isKeyDown;
			            break;
		            case SDLK_A:
			            aPressed = isKeyDown;
			            break;
                    case SDLK_S:
			            sPressed = isKeyDown;
			            break;
                    case SDLK_D:
                        dPressed = isKeyDown;
                        break;
                    case SDLK_SPACE:
			            spacePressed = isKeyDown;
                        break;
                    case SDLK_C:
			            cPressed = isKeyDown;
			            break;
					case SDLK_E:
						ePressed = isKeyDown;
						break;
                    case SDLK_Q:
						qPressed = isKeyDown;
						break;
                }
            }
        }
        cameraRotation.y += (ePressed - qPressed) * cameraSensitivity;
		if (cameraRotation.y > 360.0f) cameraRotation.y -= 360.0f;
		if (cameraRotation.y < 0.0f) cameraRotation.y += 360.0f;
        cameraMovement = glm::vec3(dPressed - aPressed, spacePressed - cPressed, wPressed - sPressed) * cameraSpeed;

        clearFrameBuffer(frameBuffer, 0xFFFFA0A0);

	    glm::mat4 modelMatrix = glm::mat4(1.0f);
	    modelMatrix = glm::translate(modelMatrix, objectPosition);
	    modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	    modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	    modelMatrix = glm::rotate(modelMatrix, glm::radians(objectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	    modelMatrix = glm::scale(modelMatrix, objectScale);

        glm::vec3 cameraFront = glm::normalize(glm::vec3(
            cos(glm::radians(cameraRotation.y)) * cos(glm::radians(cameraRotation.x)),
            sin(glm::radians(cameraRotation.x)),
            sin(glm::radians(cameraRotation.y)) * cos(glm::radians(cameraRotation.x))
        ));

        glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFrontXZ, cameraUp));

        cameraPosition += cameraRight * cameraMovement.x;
        cameraPosition += cameraFrontXZ * cameraMovement.z;
        cameraPosition.y += cameraMovement.y;
        cameraTarget = cameraPosition + cameraFront;

        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(frameBuffer.width) / static_cast<float>(frameBuffer.height), 0.1f, 100.0f);

		glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        for (int i = 0; i < objData.faces.size(); ++i)
        {
	        ObjFace& face = objData.faces[i];
			Material& material = objData.materials[face.materialIndex];

	        glm::vec4 v0 = mvpMatrix * glm::vec4(objData.vertices[face.ClipVertexIndices[0]], 1.0f);
	        glm::vec4 v1 = mvpMatrix * glm::vec4(objData.vertices[face.ClipVertexIndices[1]], 1.0f);
	        glm::vec4 v2 = mvpMatrix * glm::vec4(objData.vertices[face.ClipVertexIndices[2]], 1.0f);

            glm::vec2 uv0 = objData.textureCoords[face.textureCoordIndices[0]];
            glm::vec2 uv1 = objData.textureCoords[face.textureCoordIndices[1]];
            glm::vec2 uv2 = objData.textureCoords[face.textureCoordIndices[2]];

			std::vector<std::array<ClipVertex, 3>> clippedTriangles = clipTriangle({ v0, uv0 }, { v1, uv1 }, { v2, uv2 });

            for (const std::array<ClipVertex, 3>&clippedTriangle : clippedTriangles)
            {
                ClipVertex clipV0 = clippedTriangle[0];
                ClipVertex clipV1 = clippedTriangle[1];
                ClipVertex clipV2 = clippedTriangle[2];

                float invW0 = 1.0f / clipV0.position.w;
                float invW1 = 1.0f / clipV1.position.w;
                float invW2 = 1.0f / clipV2.position.w;

                float depth0 = clipV0.position.z * invW0;
                float depth1 = clipV1.position.z * invW1;
                float depth2 = clipV2.position.z * invW2;

                clipV0.position /= clipV0.position.w;
                clipV1.position /= clipV1.position.w;
                clipV2.position /= clipV2.position.w;

                glm::vec2 screenV0 = glm::vec2((clipV0.position.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (clipV0.position.y + 1.0f) * 0.5f) * frameBuffer.height);
                glm::vec2 screenV1 = glm::vec2((clipV1.position.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (clipV1.position.y + 1.0f) * 0.5f) * frameBuffer.height);
                glm::vec2 screenV2 = glm::vec2((clipV2.position.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (clipV2.position.y + 1.0f) * 0.5f) * frameBuffer.height);

                drawTriangle(frameBuffer, { screenV0, clipV0.uv, invW0, depth0 }, { screenV1, clipV1.uv, invW1, depth1 }, { screenV2, clipV2.uv, invW2, depth2 }, material);
            }
		}

		SDL_UpdateTexture(frameBufferTexture, nullptr, frameBuffer.pixels.data(), frameBuffer.width * sizeof(uint32_t));

		SDL_RenderClear(renderer);

		SDL_RenderTexture(renderer, frameBufferTexture, nullptr, nullptr);

		SDL_RenderPresent(renderer);
    }

    // Ruimt variabelen op en sluit SDL af
    close();

    return exitCode;
}