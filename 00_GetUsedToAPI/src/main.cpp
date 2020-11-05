#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

constexpr int FPS = 60;
constexpr int TICKS_PER_FRAME = 1000 / FPS;

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
    
    bool bRunning = true;
    const SDL_Rect dstRect = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    SDL_Point rotateCenter = {dstRect.w * 3 / 4, dstRect.h / 4};
    
    double rotateAngle = 0;

    while (bRunning)
    {
        const Uint32 startTicks = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (SDL_EventType::SDL_QUIT == e.type)
            {
                bRunning = false;
            }
        }

        const bool* keyboardStates = (const bool*)SDL_GetKeyboardState(nullptr);
        rotateAngle -= (keyboardStates[SDL_Scancode::SDL_SCANCODE_A]) ? 4 : 0;
        rotateAngle += (keyboardStates[SDL_Scancode::SDL_SCANCODE_D]) ? 4 : 0;
        rotateCenter.y -= (keyboardStates[SDL_SCANCODE_W]) ? 4 : 0;
        rotateCenter.y += (keyboardStates[SDL_SCANCODE_S]) ? 4 : 0;


        SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(g_pRenderer);
    
        SDL_RenderCopyEx(g_pRenderer, pHelloWorld, nullptr, &dstRect, rotateAngle, &rotateCenter, SDL_FLIP_NONE);

        SDL_SetRenderDrawColor(g_pRenderer, 255,0,0,0);
        SDL_Rect rotateCenterRect = {dstRect.x + rotateCenter.x - 5, dstRect.y + rotateCenter.y - 5, 10, 10};
        SDL_RenderFillRect(g_pRenderer, &rotateCenterRect);

        SDL_RenderPresent(g_pRenderer);

        const Uint32 elapsedTicks = SDL_GetTicks() - startTicks;
        if (elapsedTicks < TICKS_PER_FRAME)
        {
            SDL_Delay(TICKS_PER_FRAME - elapsedTicks);
        }
    }
    
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
