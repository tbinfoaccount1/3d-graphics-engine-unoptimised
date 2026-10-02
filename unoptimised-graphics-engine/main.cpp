#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

#include "objLoader.h"


// Schermgrootte
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };


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

		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
    }

    // Ruimt variabelen op en sluit SDL af
    close();

    return exitCode;
}