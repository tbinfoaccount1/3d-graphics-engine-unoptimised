#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "objLoader.h"
#include "rasterizer.h"


// Schermgrootte
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

FrameBuffer frameBuffer(kScreenWidth, kScreenHeight);

glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);


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
        }

        clearFrameBuffer(frameBuffer, 0xFFFFA0A0);

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(frameBuffer.width) / static_cast<float>(frameBuffer.height), 0.1f, 100.0f);

		glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		for (int i = 0; i < objData.faces.size(); ++i)
		{
			ObjFace& face = objData.faces[i];

			glm::vec4 v0 = mvpMatrix * glm::vec4(objData.vertices[face.vertexIndices[0]], 1.0f);
			glm::vec4 v1 = mvpMatrix * glm::vec4(objData.vertices[face.vertexIndices[1]], 1.0f);
			glm::vec4 v2 = mvpMatrix * glm::vec4(objData.vertices[face.vertexIndices[2]], 1.0f);

			v0 /= v0.w;
			v1 /= v1.w;
            v2 /= v2.w;

			glm::vec2 screenV0 = glm::vec2((v0.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (v0.y + 1.0f) * 0.5f) * frameBuffer.height);
			glm::vec2 screenV1 = glm::vec2((v1.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (v1.y + 1.0f) * 0.5f) * frameBuffer.height);
			glm::vec2 screenV2 = glm::vec2((v2.x + 1.0f) * 0.5f * frameBuffer.width, (1.0f - (v2.y + 1.0f) * 0.5f) * frameBuffer.height);

			drawTriangle(frameBuffer, screenV0, screenV1, screenV2, 0xFF00FF00);
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