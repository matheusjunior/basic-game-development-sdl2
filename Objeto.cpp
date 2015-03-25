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

void Objeto::desenha(SDL_Renderer *gRenderer) {
    currentTime = SDL_GetTicks();
    Update();
    SDL_RenderCopy(gRenderer, texture, NULL, &posicao);
    timeStart = currentTime;
}

