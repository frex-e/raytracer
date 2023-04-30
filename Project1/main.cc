#include <SDL.h>
#include "render.h"
#include "state.h"
#include "utility.h"
#include "sphere.h"
#include "lambertian.h"
#include <iostream>
#include <chrono>


const int SCALE = 1;
const int SCREEN_WIDTH = 1920/SCALE;
const int SCREEN_HEIGHT = 1080/SCALE;


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
State* RSTATE = NULL;

uint32_t* pixel_bffr;
uint32_t* pixels;

void printFPS()
{
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps; fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 })
    {
        oldTime = std::chrono::high_resolution_clock::now();
        std::cout << "FPS: " << fps << std::endl;
        fps = 0;
    }
}

bool init()
{
    // SDL Init Stuf
    window = SDL_CreateWindow("Title", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH*SCALE, 
        SCREEN_HEIGHT*SCALE, 
        SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) 
    { 
        std::cout << "Window could not be created! Error: " << SDL_GetError();
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        std::cout << "Renderer could not be created! Error: " << SDL_GetError();
        return false;
    }
    SDL_RenderSetScale(renderer, SCALE, SCALE);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL)
    {
        std::cout << "texture could not be created! Error: " << SDL_GetError();
        return false;
    }

    SDL_SetWindowFullscreen(window,0);
    
    // Allocate memory for pixels and initialize color
    pixels = (uint32_t*) malloc(SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
    {
       pixels[i] = (255 << 24);
    }

    RSTATE = new State(SCREEN_WIDTH,SCREEN_HEIGHT);
    
    auto material_ground = make_shared<Lambertian>(Vec3(0.6, 0.6, 0));
    auto material_centre = make_shared<Lambertian>(Vec3(0.7, 0.3, 0.7));
    auto material_gray = make_shared<Lambertian>(Vec3(0.7, 0.7, 0.7));

    RSTATE->objects.add(make_shared<Sphere>(Vec3(0,-0.4,-1),0.5,material_centre));
    RSTATE->objects.add(make_shared<Sphere>(Vec3(0.5,-0.6,-0.7),0.3,material_ground));
    RSTATE->objects.add(make_shared<Sphere>(Vec3(-0.2, -100.5, -1), 100,material_gray));
    
    return true;
}

void close()
{
    delete(RSTATE);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free(pixels);
}

void handelInput(SDL_Event e, bool *quit)
{
    if (e.type == SDL_QUIT) *quit = true;

    switch (e.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        *quit = true;
        break;
    default:
        break;
    }
}


int main(int argc, char* args[]) {

	int pitch = 0;
    int tick = 0;

    if (!init())
    {
        close();
        exit(2);
    }


	// Keeps window to stay up.

    MultiRenderer multiRenderer(12, pixels, RSTATE);

	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
        // Input Loop
		while (SDL_PollEvent(&e))
		{   
            handelInput(e, &quit);
		}

        // Render

		// Initialize render loop
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        multiRenderer.render(100);

		SDL_LockTexture(texture, NULL, (void**)&pixel_bffr, &pitch);
		pitch /= sizeof(uint32_t);
        std::copy_n(pixels, SCREEN_HEIGHT * SCREEN_WIDTH, pixel_bffr);


		SDL_UnlockTexture(texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
        
        printFPS();
	}

    close();
    return 0;

}
