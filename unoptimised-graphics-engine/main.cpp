/* Headers */
//Using SDL and STL string
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <glm/glm.hpp>


/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

bool init();
void close();



SDL_Window* window{ nullptr };
SDL_Surface* screenSurface{ nullptr };


/* Function Implementations */
bool init()
{
    //Initialization flag
    bool success{ true };

    //Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        if (window = SDL_CreateWindow("3D Graphics Engine - unoptimised", kScreenWidth, kScreenHeight, 0); window == nullptr)
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            SDL_Log("Window created");
            screenSurface = SDL_GetWindowSurface(window);
        }
    }

    return success;
}


void close()
{
    SDL_DestroyWindow(window);
    window = nullptr;
    screenSurface = nullptr;

    //Quit SDL subsystems
    SDL_Quit();
}


int main(int argc, char* args[])
{
    int exitCode{ 0 };

    int blueCount{ 240 };

	constexpr int framesBeforeBlueChange{ 500 };
	int currentFrameCount{ 0 };

    // init maakt een window aan met SDL en koppelt screenSurface aan de window
    if (!init())
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    }
    else
    {
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

			currentFrameCount++;
            if (currentFrameCount >= framesBeforeBlueChange)
            {
                currentFrameCount = 0;
				blueCount = (blueCount + 200) % 256;
            }

            // SDL functies voor het vullen van een rect (zo groot als het scherm) met een kleur en het updaten van de window
            SDL_FillSurfaceRect(screenSurface, nullptr, SDL_MapSurfaceRGB(screenSurface, 140, 140, blueCount));

            SDL_UpdateWindowSurface(window);
        }
    }

    // Ruimt variabelen op en sluit SDL af
    close();

    return exitCode;
}