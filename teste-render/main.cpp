#include <SDL2/SDL.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define RECT_WIDTH 30
#define RECT_HEIGHT 30

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gHelloWorld = NULL;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) return false;

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    return true;
}

bool loadMedia()
{
    SDL_Surface *surface = SDL_LoadBMP("media/blue-square.bmp");
    gHelloWorld = SDL_CreateTextureFromSurface(gRenderer, surface);
    return (gHelloWorld != NULL);
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char *args[])
{
    bool quit = false;
    SDL_Event e;
    SDL_Surface *aux = NULL;
    SDL_Rect SrcR;

    SrcR.x = 10;
    SrcR.y = 10;
    SrcR.w = RECT_WIDTH;
    SrcR.h = RECT_HEIGHT;

    if (!init())
    {
        puts("Falhou init\n");
        return 0;
    }

    if (!loadMedia())
    {
        puts("Falhou media\n");
        return 0;
    }

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        SrcR.y -= 5;
                        if (SrcR.y < 0) SrcR.y = 0;
                        break;
                    case SDLK_DOWN:
                        SrcR.y += 5;
                        if (SrcR.y + RECT_HEIGHT > SCREEN_HEIGHT) SrcR.y = SCREEN_HEIGHT - RECT_HEIGHT;
                        break;
                    case SDLK_LEFT:
                        SrcR.x -= 5;
                        if (SrcR.x < 0) SrcR.x = 0;
                        break;
                    case SDLK_RIGHT:
                        SrcR.x += 5;
                        if (SrcR.x + RECT_WIDTH > SCREEN_WIDTH) SrcR.x = SCREEN_WIDTH - RECT_WIDTH;
                        break;
                    default:
                        break;
                }
            }
        }
        SDL_RenderClear(gRenderer);
        // ==>  modifique o ultimo parametro NULL !!
        SDL_RenderCopy(gRenderer, gHelloWorld, NULL, &SrcR);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000);
    }

    close();

    return 0;
}