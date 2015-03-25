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
#include <vector>

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

int *posicoesLivres(Objeto *lista[]) {
    int *p = new int[50];

    for (int i = 0; i < 50; i++) p[i] = -1;

    for (int i = 0; i < 50; i++) {
        if (lista[i] != NULL) {
            p[i] = i;
        }
    }
    return p;
}

bool contem(int *p, int n) {
    return p[n] == -1;
}

bool listaVazia(Objeto *lista[]) {
    for (int i = 0; i < 50; i++) {
        if (lista[i] != NULL) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *args[]) {
    bool quit = false;
    SDL_Event e;

    srand(time(NULL));


    if(!init()) {
        cout << "Falhou init\n";
        return 0;
    }

    Canhao *cCanhao = new Canhao(SCREEN_WIDTH / 2 , SCREEN_HEIGHT - 180, 150, 165, 10);
    Objeto *cAranha = new Objeto(SCREEN_WIDTH - 50,         25         , 100,  80, 10);
    Objeto *cMosca = new Objeto(SCREEN_WIDTH / 6  , SCREEN_HEIGHT / 6  ,  54,  67, 10);


    Objeto *cTiro[LIMITE_TIROS];
    int size = 0;
    int k = 0;
    bool  tirosDisparados = false;

    cCanhao->texture = getTexture("media/cannon.bmp");
    cAranha->texture = getTexture("media/spider.bmp");
    cMosca->texture = getTexture("media/fly.bmp");

    for (int i = 0; i < LIMITE_TIROS; i++) {
        cTiro[i] = NULL;
    }

    while (!quit) {

        if (cAranha->posicao.x > SCREEN_WIDTH) cAranha->posicao.x = 0;
        else  cAranha->moverX(28);

        if (!listaVazia(cTiro)) {

            for (int j = 0; j < LIMITE_TIROS; j++)
                if (cTiro[j] != NULL) {
                    if (cTiro[j]->posicao.y < 0) {
                        delete cTiro[j];
                        cTiro[j] = NULL;
                        j--;
                        printf("===");
                        size--;
                    } else {
                        cTiro[j]->moverY(-20);
                    }
                }
        }
        SDL_Delay(5);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
                switch (e.key.keysym.sym) {

                case SDLK_SPACE:
                    k = 0;
                    if(cTiro[k] == NULL) {
                        k++;
                    }

                    cTiro[size] = new Objeto(cCanhao->posicao.x , cCanhao->posicao.y - 25, 41, 41, 10);
                    cTiro[size]->texture = getTexture("media/tiro.bmp");
                    size++;
                    tirosDisparados = true;
                    break;
                case SDLK_RIGHT:
                    cCanhao->posicao.x += 7;
                    if (cCanhao->posicao.x + cCanhao->posicao.w > SCREEN_WIDTH) cCanhao->posicao.x = SCREEN_WIDTH - cCanhao->posicao.w;
                    break;
                case SDLK_LEFT:
                    cCanhao->posicao.x -= 7;
                    if (cCanhao->posicao.x + cCanhao->posicao.w > SCREEN_WIDTH) cCanhao->posicao.x = SCREEN_WIDTH - cCanhao->posicao.w;
                    break;
                default:
                    break;
                }
            }
            cCanhao->handleInput(e);
        }

//        clear renderer and apply texture to rect
        SDL_RenderClear(gRenderer);

        cCanhao->desenha(gRenderer);
        //cMosca->desenha(gRenderer);
        cAranha->desenha(gRenderer);

        if (tirosDisparados) {
            for (int i = 0; i < LIMITE_TIROS; i++) {
                if (cTiro[i] != NULL)
                cTiro[i]->desenha(gRenderer);
            }
        }



        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}

