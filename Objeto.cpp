#include "Objeto.h"

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

Objeto::Objeto(int x, int y, int w, int h, double vel, SDL_Renderer *gRend, std::string path) {
    posicao.x = x;
    posicao.y = y;
    posicao.w = w;
    posicao.h = h;

    velX = vel;
    velY = vel;

    timeStart =0;
    currentTime =0;
    deltaT = 0;

    oRenderer = gRend;
    texture = getTexture(path);
}


SDL_Texture *Objeto::getTexture(std::string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    SDL_Texture *t = SDL_CreateTextureFromSurface(oRenderer, surface);

    if (t == NULL) exit(0);

    return t;
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
    posicao.y += (velY * y) / 50;
}

void Objeto::Update() {
}

void Objeto::desenha() {
    currentTime = SDL_GetTicks();
    Update();
    SDL_RenderCopy(oRenderer, texture, NULL, &posicao);
    timeStart = currentTime;
}

