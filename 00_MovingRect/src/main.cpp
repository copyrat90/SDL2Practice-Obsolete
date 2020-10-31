#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

constexpr char WINDOW_TITLE[] = "00_MovingRect";

SDL_Window* g_pWindow;
SDL_Renderer* g_pRenderer;
SDL_Texture* g_pBackground;

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

bool loadAsset(std::string a_path)
{
    SDL_Surface* pLoadedSurface = IMG_Load(a_path.c_str());
    if (!pLoadedSurface)
    {
        printf("loadAsset(): IMG_Load(%s) failed. IMG Error: %s\n", a_path.c_str(), IMG_GetError());
        return false;
    }
    g_pBackground = SDL_CreateTextureFromSurface(g_pRenderer, pLoadedSurface);
    if (!g_pBackground)
    {
        printf("loadAsset(): SDL_CreateTextureFromSurface() failed. SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool loadAllAssets()
{
    bool result = loadAsset("asset/image/bg.png");
    return result;
}

void render()
{
    SDL_RenderCopy(g_pRenderer, g_pBackground, nullptr, nullptr);

    SDL_SetRenderDrawColor(g_pRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect outlineRect {SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3};
    SDL_RenderDrawRect(g_pRenderer, &outlineRect);

    SDL_SetRenderDrawColor(g_pRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(g_pRenderer, SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*5/6, SCREEN_HEIGHT*5/6);

    SDL_SetRenderDrawColor(g_pRenderer, 0x00, 0xFF, 0x00, 0xFF);
    float x = SCREEN_WIDTH * 5 / 6;
    float y = SCREEN_HEIGHT / 6;
    while (x >= SCREEN_WIDTH / 6 && y <= SCREEN_HEIGHT * 5 / 6)
    {
        x -= 4.0;
        y += 4.0 * SCREEN_HEIGHT / SCREEN_WIDTH;
        SDL_RenderDrawPointF(g_pRenderer, x, y);
    }
}

int main(int argc, char* args[])
{
    if (!init())
    {
        printf("init() failed.");
        return -1;
    }

    if (!loadAllAssets())
    {
        printf("loadAllAssets() failed.");
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

        SDL_RenderClear(g_pRenderer);
        render();
        SDL_RenderPresent(g_pRenderer);
    }
    
    return 0;
}