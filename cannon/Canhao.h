
#ifndef c
#define c

#import <SDL2/SDL_events.h>
#import "Objeto.h"
#include <list>
#include <vector>

class Canhao : public Objeto {
    std::vector<SDL_Rect> bullets;
public:
    Canhao(int x, int y, int w, int h, double vel);

    void handleInput(SDL_Event e);
    void desenha(SDL_Renderer *gRenderer) override;
    void fire();
};

Canhao::Canhao(int x, int y, int w, int h, double vel) : Objeto(x,y,w,h,vel) {}

void Canhao::desenha(SDL_Renderer *gRenderer) {
    currentTime = SDL_GetTicks();
    //Update();
    if (NULL == texture) exit(9);

    SDL_RenderCopy(gRenderer, texture, NULL, &posicao);

//    update bullets positions
    for (int i = 0; i < bullets.size(); ++i) bullets[i].y -= 10;

//    remove off-screen bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        if (bullets[i].y < 0) bullets.erase(bullets.begin() + i);
    }

//    render bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        SDL_RenderCopy(gRenderer, texture, NULL, &bullets[i]);
    }

    timeStart = currentTime;
}

void Canhao::handleInput(SDL_Event e) {

    switch (e.key.keysym.sym) {

    case SDLK_SPACE:
        break;
    case SDLK_RIGHT:
        posicao.x += 7;
        if (posicao.x + posicao.w > SCREEN_WIDTH) posicao.x = SCREEN_WIDTH - posicao.w;
        break;
    case SDLK_LEFT:
        posicao.x -= 7;
        if (posicao.x + posicao.w > SCREEN_WIDTH) posicao.x = SCREEN_WIDTH - posicao.w;
        break;
    default:
        break;
    }
}

void Canhao::fire()
{
    SDL_Rect rect;
    rect.x = this->posicao.x;
    rect.y = this->posicao.y - posicao.h;
    rect.h = 50;
    rect.w = 50;
    this->bullets.push_back(rect);
}
#endif // canhao
