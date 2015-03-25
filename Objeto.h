#ifndef objeto
#define objeto

#include "Consts.h"

#ifdef _WIN32
#include <SDL.h>
#else

#include <SDL2/SDL.h>

#endif

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
};

#endif // objeto
