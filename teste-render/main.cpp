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
SDL_Texture *gTextureGreenBox = NULL;

using namespace std;

//bool DEBUG = false;

// init SDL video component and create main window
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gWindow = SDL_CreateWindow("Testando !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL) return false;
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
    SDL_Surface *surfaceGreenBox = SDL_LoadBMP("media/green1-square.bmp");
    gTextureGreenBox = SDL_CreateTextureFromSurface(gRenderer, surfaceGreenBox);

    return (gTextureGreenBox != NULL);
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
        if(rect->y < 0) rect->y = 0;
    }
//    move down
    else if(n >= 0.26 && n <= 0.5)
    {
        rect->y += 1;
        if(rect->y + RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT - RECT_HEIGHT;
    }
//    move left
    else if(n >= 0.51 && n <= 0.75)
    {
        rect->x -= 1;
        if(rect->x < 0) rect->x = 0;
    }
//    move right
    else
    {
        rect->x += 1;
        if(rect->x + RECT_WIDTH > SCREEN_WIDTH) rect->x = SCREEN_WIDTH - RECT_WIDTH;
    }
}

int main(int argc, char *args[])
{
    bool quit = false;
    SDL_Event e;
    SDL_Rect squares[10];
    bool selectedSquares[10];

    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        squares[i].x = rand() % SCREEN_WIDTH;
        squares[i].y = rand() % SCREEN_HEIGHT;
        squares[i].w = 30;
        squares[i].h = 30;
        selectedSquares[i] = false;
    }

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
//        event handler
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_MOUSEMOTION)
            {
                for (int i = 0; i < 10; ++i)
                {
                    selectedSquares[i] = isMouseInRect(squares[i], e.motion.x, e.motion.y) ? false : true;
                }
            }
        }

//        clear renderer and apply texture to rect
        SDL_RenderClear(gRenderer);

        for (int i = 0; i < 10; i++)
        {
            if(selectedSquares[i]) SDL_RenderCopy(gRenderer, gTextureGreenBox, NULL, &squares[i]);
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(30);
    }
    close();
    return 0;
}
