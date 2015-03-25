
#include <SDL.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <math.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define RECT_WIDTH 35
#define RECT_HEIGHT 35

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTextureBlueBox = NULL;
SDL_Texture *gTextureYellowBox = NULL;
SDL_Texture *gTextureGreeBoxA = NULL;
SDL_Texture *gTextureGreeBoxB = NULL;
SDL_Texture *gTexturePurpleBox = NULL;


bool DEBUG = false;

using namespace std;

// init SDL video component and create main window
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) return false;

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

    return true;
}

bool loadMedia(char *path) {
    SDL_Surface *surface = SDL_LoadBMP(path);

    gTextureBlueBox = SDL_CreateTextureFromSurface(gRenderer, surface);

    return (gTextureBlueBox != NULL);
}

bool loadMedia() {
    SDL_Surface *surfaceBlueBox = SDL_LoadBMP("media/blue-square.bmp");
    SDL_Surface *surfaceYellowBox = SDL_LoadBMP("media/yellow-square.bmp");
    SDL_Surface *surfaceGreenBoxA = SDL_LoadBMP("media/green1-square.bmp");
    SDL_Surface *surfaceGreenBoxB = SDL_LoadBMP("media/green2-square.bmp");
    SDL_Surface *surfacePurpleBox = SDL_LoadBMP("media/purple-square.bmp");

    gTextureBlueBox = SDL_CreateTextureFromSurface(gRenderer, surfaceBlueBox);

    gTextureYellowBox = SDL_CreateTextureFromSurface(gRenderer, surfaceYellowBox);

    gTextureGreeBoxA = SDL_CreateTextureFromSurface(gRenderer, surfaceGreenBoxA);

    gTextureGreeBoxB = SDL_CreateTextureFromSurface(gRenderer, surfaceGreenBoxB);

    gTexturePurpleBox = SDL_CreateTextureFromSurface(gRenderer, surfacePurpleBox);

    return (gTextureBlueBox != NULL && gTextureYellowBox != NULL && gTextureGreeBoxA != NULL && gTextureGreeBoxB != NULL && surfacePurpleBox != NULL);
}

void close() {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

// get mouse x and y position
// returns true if mouse position is in rect, false otherwise
bool isMouseInRect(SDL_Rect rect, int mX, int mY) {
    return (mX >= rect.x && mX <= rect.x + rect.w &&
            mY >= rect.y && mY <= rect.y + rect.h) ? true : false;
}

double generateRandom(int l, int u) {
    srand(time(NULL));

    double r = rand() % ((u - l) + 1);
    r = l + r;

    return r;
}

void moveRectPseudo(SDL_Rect *rect) {

    double n = generateRandom(1,4);

    if (DEBUG) {
        cout << n << endl;
    }
    double step = generateRandom(1,2);


//    move up
    if (n == 1) {
        rect->y -= step;
        if (rect->y < 0) rect->y = 0;
    }
//    move down
    else if (n == 2) {
        rect->y += step;
        if (rect->y + RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT - RECT_HEIGHT;
    }
//    move left
    else if (n == 3) {
        rect->x -= step;
        if (rect->x < 0) rect->x = 0;
    }
//    move right
    else {
        rect->x += step;
        if (rect->x + RECT_WIDTH > SCREEN_WIDTH) rect->x = SCREEN_WIDTH - RECT_WIDTH;
    }
}

// move rect randomly on screen
void moveRect(SDL_Rect *rect) {
//    int random_integer = rand();
//    srandom((unsigned int) clock());
    double n = rand();
    n = n / RAND_MAX;

//    move up
    if (n <= 0.25) {
        rect->y -= 1;
        if (rect->y < 0) rect->y = 0;
    }
//    move down
    else if (n >= 0.26 && n <= 0.5) {
        rect->y += 1;
        if (rect->y + RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT - RECT_HEIGHT;
    }
//    move left
    else if (n >= 0.51 && n <= 0.75) {
        rect->x -= 1;
        if (rect->x < 0) rect->x = 0;
    }
//    move right
    else {
        rect->x += 1;
        if (rect->x + RECT_WIDTH > SCREEN_WIDTH) rect->x = SCREEN_WIDTH - RECT_WIDTH;
    }
}

int main(int argc, char *args[]) {
    bool quit = false;
    bool greenAselected = true;

    SDL_Event e;
    SDL_Rect rect;
    SDL_Rect rect2;
    SDL_Rect rect3;
    SDL_Rect rect4;


    rect.x = SCREEN_WIDTH / 2;
    rect.y = SCREEN_HEIGHT / 2;
    rect.w = RECT_WIDTH;
    rect.h = RECT_HEIGHT;


    rect2.x = SCREEN_WIDTH / 3;
    rect2.y = SCREEN_HEIGHT / 3;
    rect2.w = RECT_WIDTH;
    rect2.h = RECT_HEIGHT;

    rect3.x = SCREEN_WIDTH / 3 + 30;
    rect3.y = SCREEN_HEIGHT / 3 + 100;
    rect3.w = RECT_WIDTH;
    rect3.h = RECT_HEIGHT;

    rect4.x = SCREEN_WIDTH / 3 + 190;
    rect4.y = SCREEN_HEIGHT / 3 + 110;
    rect4.w = RECT_WIDTH;
    rect4.h = RECT_HEIGHT;

    if (!init()) {
        cout << "Falhou init\n";
        return 0;
    }

    if (!loadMedia()) {
        cout << "Falhou media\n";
        return 0;
    }
//    game loop
    while (!quit) {
//        event handler

        moveRectPseudo(&rect2);
        moveRect(&rect3);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    rect.y -= 5;
                    if (rect.y < 0) rect.y = 0;
                    SDL_SetWindowTitle(gWindow, "Subindo");
                    break;
                case SDLK_DOWN:
                    rect.y += 5;
                    if (rect.y + RECT_HEIGHT > SCREEN_HEIGHT) rect.y = SCREEN_HEIGHT - RECT_HEIGHT;
                    SDL_SetWindowTitle(gWindow, "Descendo");
                    break;
                case SDLK_LEFT:
                    rect.x -= 5;
                    if (rect.x < 0) {
                        rect.x = 0;
                        SDL_SetWindowTitle(gWindow, "Limite identificado");
                    } else {
                        SDL_SetWindowTitle(gWindow, "Esquerda");
                    }
                    break;
                case SDLK_RIGHT:
                    rect.x += 5;
                    if (rect.x + RECT_WIDTH > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - RECT_WIDTH;
                    SDL_SetWindowTitle(gWindow, "Direita");
                    break;
                default:
                    break;
                }
            } else if (e.type == SDL_MOUSEMOTION) while (isMouseInRect(rect4, e.motion.x, e.motion.y)) moveRect(&rect4);

            if (isMouseInRect(rect3,e.motion.x,e.motion.y)) {
                greenAselected = false;
            } else {
                greenAselected = true;
            }

        }
//        clear renderer and apply texture to rect
        SDL_RenderClear(gRenderer);
        // ==>  modifique o ultimo parametro NULL !!
        SDL_RenderCopy(gRenderer, gTextureBlueBox, NULL, &rect);
        SDL_RenderCopy(gRenderer, gTextureYellowBox, NULL, &rect2);

        if (greenAselected) {
            SDL_RenderCopy(gRenderer, gTextureGreeBoxA, NULL, &rect3);
        } else {
            SDL_RenderCopy(gRenderer, gTextureGreeBoxB, NULL, &rect3);
        }
        SDL_RenderCopy(gRenderer, gTexturePurpleBox, NULL, &rect4);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(30);
    }
    close();

    return 0;
}
