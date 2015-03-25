#ifndef objeto
#define objeto

#include "Consts.h"

class Objeto {
public:

    SDL_Texture *texture;
    SDL_Rect posicao;
    double velX;
    double velY;
    double timeStart;
    double currentTime;
    double deltaT;

    Objeto () {;}
    Objeto(int x, int y, int w, int h);
    Objeto(int x, int y, int w, int h, double vel);

    void moverX(double x);
    void moverY(double y);

    void setVelocidadeX(double v);
    void setVelocidadeY(double v);

    void setTexture(SDL_Texture *tex);

    void desenha(SDL_Renderer *gRenderer);

    void Update();

    void handleInput(SDL_Event e);
//private:
   // SDL_Surface surface;
};


Objeto::Objeto(int x, int y, int w, int h) {
    posicao.x = x;
    posicao.y = y;
    posicao.w = w;
    posicao.h = h;

    velX = 10;
    velY = 10;

    texture = NULL;

    timeStart =0;
    currentTime =0;
    deltaT = 0;
}

Objeto::Objeto(int x, int y, int w, int h, double vel) {
    posicao.x = x;
    posicao.y = y;
    posicao.w = w;
    posicao.h = h;

    velX = vel;
    velY = vel;

    texture = NULL;

    timeStart =0;
    currentTime =0;
    deltaT = 0;
}


void Objeto::setVelocidadeX(double v) {
    velX = v;
}

void Objeto::setVelocidadeY(double v) {
    velY = v;
}

void Objeto::setTexture(SDL_Texture *tex) {
    texture = tex;
}

void Objeto::moverX(double x) {
    posicao.x += (velX * x) / 100;
}

void Objeto::moverY(double y) {
    posicao.y += velY * y;
}

void Objeto::Update() {
    posicao.x += velX * (currentTime - timeStart) / 1000;
}

void Objeto::desenha(SDL_Renderer *gRenderer) {
    currentTime = SDL_GetTicks();
    Update();
    if (NULL ==texture) exit(9);
    SDL_RenderCopy(gRenderer, texture, NULL, &posicao);
    timeStart = currentTime;
}

void Objeto::handleInput(SDL_Event e) {

    switch (e.key.keysym.sym) {
    case SDLK_UP:
        posicao.y -= 5;
        if (posicao.y < 0) posicao.y = 0;
        break;
    case SDLK_DOWN:
        posicao.y += 5;
        if (posicao.y + posicao.h > SCREEN_HEIGHT) posicao.y = SCREEN_HEIGHT - posicao.h;
        break;
    case SDLK_LEFT:
        posicao.x -= 5;
        if (posicao.x < 0) {
            posicao.x = 0;
        }
        break;
    case SDLK_RIGHT:
        posicao.x += 5;
        if (posicao.x + posicao.w > SCREEN_WIDTH) posicao.x = SCREEN_WIDTH - posicao.w;
        break;
    default:
        break;
    }
}

#endif // objeto
