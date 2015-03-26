#ifndef objeto
#define objeto

#include "Consts.h"
#include "string"


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
    SDL_Renderer *oRenderer;


    Objeto () {;}
    Objeto(int x, int y, int w, int h);
    Objeto(int x, int y, int w, int h, double vel);
    Objeto(int x, int y, int w, int h, double vel, SDL_Renderer *gRend, std::string path);

    void moverX(double x);
    void moverY(double y);

    void setVelocidadeX(double v);
    void setVelocidadeY(double v);

    void setTexture(SDL_Texture *tex);

    void desenha();

    SDL_Texture *getTexture(std::string path);

    void changeTexture(std::string path);

    void Update();

    bool Collide(SDL_Rect obj2);
private:
    int color;
};

#endif // objeto
