#ifdef _WIN32
#include <SDL.h>
#include <windows.h>
#endif // _WIN32

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <math.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 400
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

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter() {
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

void f1(SDL_Rect *rect) {
    Uint32 start;

    int temp = 0;

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    Uint32 realStart = 0;
    Uint32 sumTime = 0;

    cout << asctime (timeinfo) << endl;
    int a = 0;

    for (int i = 0; i < SCREEN_HEIGHT; i += 1) {

        start = SDL_GetTicks();

        rect->y += 1;
        if (rect->y - RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT;
        if (rect->y == SCREEN_HEIGHT) {
            rect->y = 0;
            a++;
        }

        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, gTextureBlueBox, NULL, rect);

        SDL_RenderPresent(gRenderer);

//            cout << k++ << " * " << GetCounter() << endl;

        Uint32 timeRunning = SDL_GetTicks();



        //cout << "\n\nRunning\t" << timeRunning << "\nStart\t" << start << "\nDiff\t" << timeRunning - start;

        if (realStart == 0) {
            realStart = start;
        }
        int nCircles = 0;
        while ( timeRunning - start < 2.5) {
            timeRunning = SDL_GetTicks();
            nCircles++;

        }
 //       cout << "\tN. of Circles= " << nCircles << " Diff = "  << timeRunning - start << endl;

        temp++;
        sumTime += timeRunning;


        if (rect->y == 0 && a == 2) {

            timeinfo = localtime ( &rawtime );
            cout << "Time Running\t" << sumTime << endl;
            cout << "Start\t\t" << realStart << endl;
            cout << "Diff\t\t " << sumTime - realStart << endl;
            cout <<  asctime (timeinfo) << endl;
            cin >> temp;
        }
        start = timeRunning;


    }

}


void f2(SDL_Rect *rect) {

    double velocity = 3;

    StartCounter();

    float timeStep = GetCounter() / 10.f;

    rect->y += velocity * timeStep;

    if (rect->y - RECT_HEIGHT > SCREEN_HEIGHT) rect->y = SCREEN_HEIGHT;
    if (rect->y == SCREEN_HEIGHT) {
        rect->y = 0;
    }

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gTextureBlueBox, NULL, rect);

    SDL_RenderPresent(gRenderer);
}

int main(int argc, char *args[]) {
    bool quit = false;

    SDL_Event e;
    SDL_Rect rect;

    rect.x = SCREEN_WIDTH / 2 - (RECT_WIDTH / 2);
    rect.y = SCREEN_HEIGHT / 2;
    rect.w = RECT_WIDTH;
    rect.h = RECT_HEIGHT;

    if (!init()) {
        cout << "Falhou init\n";
        return 0;
    }

    if (!loadMedia()) {
        cout << "Falhou media\n";
        return 0;
    }


    while (!quit) {


        f1(&rect);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }
    }
    close();

    return 0;
}
