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

    color = 0;
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

    color = 255;
}


SDL_Texture *Objeto::getTexture(std::string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    SDL_Texture *t = SDL_CreateTextureFromSurface(oRenderer, surface);

    if (t == NULL) exit(0);

    return t;
}

bool Objeto::Collide(SDL_Rect obj2) {

    int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;

	left1 = posicao.x;
	left2 = obj2.x;
	right1 = posicao.x + posicao.w;
	right2 = obj2.x + obj2.w;
	top1 = posicao.y;
	top2 = obj2.y;
	bottom1 = posicao.y + posicao.h;
	bottom2 = obj2.y + obj2.h;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;

	if (right1 < left2) return false;
	if (left1 > right2) return false;

    SDL_SetRenderDrawColor(oRenderer, 255, 0, 0, 0);

	return true;
}

void Objeto::changeTexture(std::string path) {
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    SDL_Texture *t = SDL_CreateTextureFromSurface(oRenderer, surface);

    if (t == NULL) exit(0);
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
    if (color + 1 < 255) color++;
    SDL_SetRenderDrawColor(oRenderer, 255, color, color, color);
}

void Objeto::desenha() {
    currentTime = SDL_GetTicks();
    SDL_RenderCopy(oRenderer, texture, NULL, &posicao);
    timeStart = currentTime;
    Update();
}

