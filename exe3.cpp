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
    int *p = new int[LIMITE_TIROS];

    for (int i = 0; i < LIMITE_TIROS; i++) p[i] = -1;

    for (int i = 0; i < LIMITE_TIROS; i++) {
        if (lista[i] != NULL) {
            p[i] = i;
        }
    }
    return p;
}

int numeroPosicoesLivre(Objeto *lista[]) {
    int n = 0;

    for (int i = 0; i < LIMITE_TIROS; i++) {
        if (lista[i] == NULL) n++;
    }

    return n;
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

    Objeto *cCanhao = new Objeto((SCREEN_WIDTH / 2) - 150/2, SCREEN_HEIGHT - 180, 150, 165, 10, gRenderer, "media/cannon.bmp");
    Objeto *cAranha = new Objeto(         SCREEN_WIDTH - 50,                  25, 100,  80, 10, gRenderer, "media/spider.bmp");
//    Objeto *cMosca = new Objeto(           SCREEN_WIDTH / 6,   SCREEN_HEIGHT / 6,  54,  67, 10, gRenderer, "media/fly.bmp");
    Objeto *cListaDeTiros[LIMITE_TIROS];

    int size = 0;
    int k = 1;
    int qtdTiros = 0;

    for (int i = 0; i < LIMITE_TIROS; i++) {
        cListaDeTiros[i] = NULL;
    }

    while (!quit) {

        if (cAranha->posicao.x > SCREEN_WIDTH) cAranha->posicao.x = 0;
        else  cAranha->moverX(28);

        if (!listaVazia(cListaDeTiros)) {

            for (int j = 0; j < LIMITE_TIROS; j++)
                if (cListaDeTiros[j] != NULL) {
                    if (cListaDeTiros[j]->posicao.y < 0) {
                        delete cListaDeTiros[j];
                        cListaDeTiros[j] = NULL;
                        j = 0;
                        printf("\nDestroidos: %d", k++);
                        size--;
                    } else {

                        cListaDeTiros[j]->moverY(-20);
                    }
                }
        }

        SDL_Delay(5);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    exit(0);
                }

                else if (e.key.keysym.sym == SDLK_SPACE) {
                    cListaDeTiros[size] = new Objeto(cCanhao->posicao.x + 57 , cCanhao->posicao.y - 25, 41, 41, 10, gRenderer, "media/tiro.bmp");
                    size++;
                    qtdTiros++;

                    cout << "\nTiros: " << qtdTiros;
                    SDL_Delay(3);
                }


                if (e.key.keysym.sym == SDLK_RIGHT) {
                    cCanhao->posicao.x += 7;
                    if (cCanhao->posicao.x + cCanhao->posicao.w > SCREEN_WIDTH) cCanhao->posicao.x = SCREEN_WIDTH - cCanhao->posicao.w;
                }


                if (e.key.keysym.sym == SDLK_LEFT) {
                    cCanhao->posicao.x -= 7;
                    if (cCanhao->posicao.x + cCanhao->posicao.w > SCREEN_WIDTH) cCanhao->posicao.x = SCREEN_WIDTH - cCanhao->posicao.w;
                }


                if (e.key.keysym.sym == SDLK_z) {
                    for (int i = 0; i < LIMITE_TIROS; i++) {
                        if (cListaDeTiros[i] != NULL) {
                            cout << endl << cListaDeTiros[i]->posicao.y;
                        }
                    }
                }

            }
        }

        SDL_RenderClear(gRenderer);

        cCanhao->desenha();
        //cMosca->desenha(gRenderer);
        cAranha->desenha();


        for (int i = 0; i < LIMITE_TIROS; i++) {
            if (cListaDeTiros[i] != NULL) {

                cListaDeTiros[i]->desenha();
                if(cListaDeTiros[i]->Collide(cAranha->posicao)) {
                    cListaDeTiros[i] = new Objeto(cListaDeTiros[i]->posicao.x , cListaDeTiros[i]->posicao.y, 41, 41, 10, gRenderer, "media/tiro2.bmp");
                }
            }
        }

//        cout << "n. posicoes livre: " << numeroPosicoesLivre(cListaDeTiros) << endl;

        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}

