#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

SDL_Window* g_pWindow;
SDL_Renderer* g_pRenderer;

bool init();
void clean();

int main(int argc, char* args[])
{
    if (!init())
    {
        clean();
        return -1;
    }

    SDL_Texture* pHelloWorld;
    {
        SDL_Surface* pTempSurface = IMG_Load("asset/image/helloworld.png");
        pHelloWorld = SDL_CreateTextureFromSurface(g_pRenderer, pTempSurface);
        SDL_FreeSurface(pTempSurface);
        pTempSurface = nullptr;
    }
    
    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(g_pRenderer);

    SDL_Rect dstRect = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    SDL_RenderCopyEx(g_pRenderer, pHelloWorld, nullptr, &dstRect, 10, nullptr, SDL_FLIP_NONE);

    SDL_RenderPresent(g_pRenderer);


    SDL_Delay(2000);
    
    clean();
    return 0;
}



bool init()
{
    if (0 > SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init() failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    if (0 == IMG_Init(IMG_INIT_PNG))
    {
        printf("IMG_Init() failed. IMG Error: %s\n", IMG_GetError());
        return false;
    }

    g_pWindow = SDL_CreateWindow("play", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (nullptr == g_pWindow)
    {
        printf("SDL_Window creation failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == g_pRenderer)
    {
        printf("SDL_Renderer creation failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void clean()
{
    SDL_DestroyRenderer(g_pRenderer);
    SDL_DestroyWindow(g_pWindow);

    IMG_Quit();
    SDL_Quit();
}
