#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>

#endif

#include <time.h>
#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define RECT_WIDTH 35
#define RECT_HEIGHT 35


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTextureBlueBox = NULL;
SDL_Texture *gTextureYellowBox = NULL;
SDL_Texture *gTextureGreenBoxA = NULL;
SDL_Texture *gTextureGreenBoxB = NULL;
SDL_Texture *gTexturePurpleBox = NULL;

using namespace std;

//bool DEBUG = false;

// init SDL video component and create main window
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) return false;
    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

    return true;
}

bool loadMedia(char *path)
{
    SDL_Surface *surface = SDL_LoadBMP(path);
    gTextureBlueBox = SDL_CreateTextureFromSurface(gRenderer, surface);

    return (gTextureBlueBox != NULL);
}

bool loadMedia()
{
    SDL_Surface *surfaceBlueBox = SDL_LoadBMP("media/blue-square.bmp");
    SDL_Surface *surfaceYellowBox = SDL_LoadBMP("media/yellow-square.bmp");
    SDL_Surface *surfaceGreenBoxA = SDL_LoadBMP("media/green1-square.bmp");
    SDL_Surface *surfaceGreenBoxB = SDL_LoadBMP("media/green2-square.bmp");
    SDL_Surface *surfacePurpleBox = SDL_LoadBMP("media/purple-square.bmp");

    gTextureBlueBox = SDL_CreateTextureFromSurface(gRenderer, surfaceBlueBox);
    gTextureYellowBox = SDL_CreateTextureFromSurface(gRenderer, surfaceYellowBox);
    gTextureGreenBoxA = SDL_CreateTextureFromSurface(gRenderer, surfaceGreenBoxA);
    gTextureGreenBoxB = SDL_CreateTextureFromSurface(gRenderer, surfaceGreenBoxB);
    gTexturePurpleBox = SDL_CreateTextureFromSurface(gRenderer, surfacePurpleBox);

    return (gTextureBlueBox != NULL && gTextureYellowBox != NULL &&
            gTextureGreenBoxA != NULL && gTextureGreenBoxB != NULL && surfacePurpleBox != NULL);
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

// get mouse x and y position
// returns true if mouse position is in rect, false otherwise
bool isMouseInRect(SDL_Rect rect, int mX, int mY)
{
    return (mX >= rect.x && mX <= rect.x + rect.w &&
            mY >= rect.y && mY <= rect.y + rect.h) ? true : false;
}

double generateRandom(int l, int u)
{
    srand(time(NULL));
    double r = rand() % ((u - l) + 1);
    r = l + r;

    return r;
}

void moveRectPseudo(SDL_Rect *rect)
{
    double n = generateRandom(1, 4);

    if(DEBUG) cout << n << endl;

    double STEP = generateRandom(1, 2);
//    move up
    if(n == 1)
    {
        rect->y -= STEP;
        if(rect->y < 0) rect->y = 0;
    }
//    move down
    else if(n == 2)
    {
        rect->y += STEP;
        if(rect->y + RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT - RECT_HEIGHT;
    }
//    move left
    else if(n == 3)
    {
        rect->x -= STEP;
        if(rect->x < 0) rect->x = 0;
    }
//    move right
    else
    {
        rect->x += STEP;
        if(rect->x + RECT_WIDTH > SCREEN_WIDTH) rect->x = SCREEN_WIDTH - RECT_WIDTH;
    }
}

// move rect randomly on screen
void moveRect(SDL_Rect *rect)
{
//    int random_integer = rand();
//    srandom((unsigned int) clock());
    double n = rand();
    n = n / RAND_MAX;

//    move up
    if(n <= 0.25)
    {
        rect->y -= 1;
        if (rect->y < 0) rect->y = 0;
    }
//    move down
    else if(n >= 0.26 && n <= 0.5)
    {
        rect->y += 1;
        if (rect->y + RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT - RECT_HEIGHT;
    }
//    move left
    else if(n >= 0.51 && n <= 0.75)
    {
        rect->x -= 1;
        if (rect->x < 0) rect->x = 0;
    }
//    move right
    else
    {
        rect->x += 1;
        if (rect->x + RECT_WIDTH > SCREEN_WIDTH) rect->x = SCREEN_WIDTH - RECT_WIDTH;
    }
}

int main(int argc, char *args[])
{
    bool quit = false;
    bool greenSelected = true;

    SDL_Event e;
    SDL_Rect rect;
    SDL_Rect yellowRect;
    SDL_Rect greenRect;
    SDL_Rect violetRect;

    rect.x = SCREEN_WIDTH / 2;
    rect.y = SCREEN_HEIGHT / 2;
    rect.w = RECT_WIDTH;
    rect.h = RECT_HEIGHT;

    yellowRect.x = SCREEN_WIDTH / 3;
    yellowRect.y = SCREEN_HEIGHT / 3;
    yellowRect.w = RECT_WIDTH;
    yellowRect.h = RECT_HEIGHT;

    greenRect.x = SCREEN_WIDTH / 3 + 30;
    greenRect.y = SCREEN_HEIGHT / 3 + 100;
    greenRect.w = RECT_WIDTH;
    greenRect.h = RECT_HEIGHT;

    violetRect.x = SCREEN_WIDTH / 3 + 190;
    violetRect.y = SCREEN_HEIGHT / 3 + 110;
    violetRect.w = RECT_WIDTH;
    violetRect.h = RECT_HEIGHT;

    if(!init())
    {
        cout << "Falhou init\n";
        return 0;
    }

    if(!loadMedia())
    {
        cout << "Falhou media\n";
        return 0;
    }
//    game loop
    while (!quit)
    {
        moveRectPseudo(&yellowRect);
        moveRect(&greenRect);
//        event handler
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
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
                        if(rect.x < 0)
                        {
                            rect.x = 0;
                            SDL_SetWindowTitle(gWindow, "Limite identificado");
                        }
                        else SDL_SetWindowTitle(gWindow, "Esquerda");
                        break;
                    case SDLK_RIGHT:
                        rect.x += 5;
                        if (rect.x + RECT_WIDTH > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - RECT_WIDTH;
                        SDL_SetWindowTitle(gWindow, "Direita");
                        break;
                    default:
                        break;
                }
            }
            else if(e.type == SDL_MOUSEMOTION) while (isMouseInRect(violetRect, e.motion.x, e.motion.y)) moveRect(&violetRect);
            if(e.type == SDL_MOUSEMOTION) greenSelected = isMouseInRect(greenRect, e.motion.x, e.motion.y) ? false : true;
        }
//        clear renderer and apply texture to rect
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTextureBlueBox, NULL, &rect);
        SDL_RenderCopy(gRenderer, gTextureYellowBox, NULL, &yellowRect);

        if(greenSelected) SDL_RenderCopy(gRenderer, gTextureGreenBoxA, NULL, &greenRect);
        else SDL_RenderCopy(gRenderer, gTextureGreenBoxB, NULL, &greenRect);
        SDL_RenderCopy(gRenderer, gTexturePurpleBox, NULL, &violetRect);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(30);
    }
    close();
    return 0;
}
