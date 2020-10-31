#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

constexpr char WINDOW_TITLE[] = "00_MovingRect";

SDL_Window* g_pWindow;
SDL_Renderer* g_pRenderer;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init() failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    g_pWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!g_pWindow)
    {
        printf("SDL_CreateWindow() failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    if (!g_pRenderer)
    {
        printf("SDL_CreateRenderer() failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_InitFlags::IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("IMG_Init() failed. IMG Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

bool loadAsset()
{
    // TODO: implement loading "asset/image/bg.png"
    return true;
}

int main(int argc, char* args[])
{
    if (!init())
    {
        printf("init() failed.");
        return -1;
    }

    if (!loadAsset())
    {
        printf("loadAsset() failed.");
        return -1;
    }

    bool bQuit = false;

    while (!bQuit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EventType::SDL_QUIT)
            {
                bQuit = true;
            }

            switch (e.key.keysym.sym)
            {
            case SDL_KeyCode::SDLK_UP:
                break;
            case SDL_KeyCode::SDLK_DOWN:
                break;
            case SDL_KeyCode::SDLK_LEFT:
                break;
            case SDL_KeyCode::SDLK_RIGHT:
                break;
            }
        }
    }
    
    return 0;
}