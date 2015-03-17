#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Texture *gHelloWorld = NULL;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) return false;

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    gScreenSurface = SDL_GetWindowSurface(gWindow);

    return true;
}

bool loadMedia()
{
    SDL_Surface *surface = SDL_LoadBMP("media/teste.bmp");
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
    SDL_Surface *aux = NULL;
    SDL_Rect SrcR;

    SrcR.x = 10;
    SrcR.y = 10;
    SrcR.w = 30;
    SrcR.h = 30;

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


    SDL_RenderClear(gRenderer);

    // ==>  modifique o ultimo parametro NULL !!
    SDL_RenderCopy(gRenderer, gHelloWorld, NULL, &SrcR);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(1000);
    close();

    return 0;
}