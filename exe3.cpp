#ifdef _WIN32
#include <SDL.h>
#else

#include <SDL2/SDL.h>

#endif

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Objeto.h"
#include "Canhao.h"
#include "Consts.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTextureCannon = NULL;
SDL_Texture *gTextureFly = NULL;
SDL_Texture *gTextureSpider = NULL;

using namespace std;

bool init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL) return false;
    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    return true;
}

SDL_Texture *getTexture(string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    SDL_Texture *t = SDL_CreateTextureFromSurface(gRenderer, surface);

    if (t == NULL) cout << SDL_GetError();

    return t;
}

void close() {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

// get mouse x and y position
// returns true if mouse position is in rect, false otherwise
bool isMouseInRect(SDL_Rect rect, int mX, int mY) {
    printf("mx %d my %d\n", mX, mY);
    return ((rect.x >= mX - 25 && rect.x <= mX + 25 && rect.y <= mY + 25 && rect.y >= mY - 25) ||
            (rect.x + rect.w >= mX - 25 && rect.x + rect.w <= mX + 25 &&
             rect.y + rect.h <= mY + 25 && rect.y + rect.h >= mY - 25)) ? true : false;
}



int main(int argc, char *args[]) {
    bool quit = false;
    SDL_Event e;

    srand(time(NULL));


    if(!init()) {
        cout << "Falhou init\n";
        return 0;
    }

    Canhao *zanhao = new Canhao(SCREEN_WIDTH / 2, SCREEN_HEIGHT -180, 150, 165, 10);
    Objeto *aranha = new Objeto(SCREEN_WIDTH - 50, 25, 100, 80, 10);
    Objeto *mosca = new Objeto(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, 54, 67, 10);


  printf (" x vale %d", zanhao->posicao.x);

    zanhao->texture = getTexture("media/cannon.bmp");
    aranha->texture = getTexture("media/spider.bmp");
    mosca->texture = getTexture("media/fly.bmp");

    while (!quit) {

        if (aranha->posicao.x > SCREEN_WIDTH) aranha->posicao.x = 0;
        else  aranha->moverX(28);
        SDL_Delay(5);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
            zanhao->handleInput(e);
        }

//        clear renderer and apply texture to rect
        SDL_RenderClear(gRenderer);

        zanhao->desenha(gRenderer);
        //mosca->desenha(gRenderer);
        aranha->desenha(gRenderer);

        SDL_RenderPresent(gRenderer);



    }
    close();
    return 0;
}

