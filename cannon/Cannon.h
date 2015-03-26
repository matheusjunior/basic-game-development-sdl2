#ifndef CANNON_H_
#define CANNON_H_

#import <SDL2/SDL_events.h>
#import "GameObject.h"
#include <list>
#include <vector>

class Cannon : public GameObject
{
public:
    std::vector<SDL_Rect> bullets;
public:
    Cannon(int x, int y, int w, int h, double vel);

    void handleInput(SDL_Event e);

    void draw(SDL_Renderer *gRenderer) override;
    void fire();
};

Cannon::Cannon(int x, int y, int w, int h, double vel) : GameObject(x, y, w, h, vel)
{
}

void Cannon::draw(SDL_Renderer *gRenderer)
{
    currentTime = SDL_GetTicks();
    //Update();
    if(texture == NULL) exit(9);
    SDL_RenderCopy(gRenderer, texture, NULL, &position);

//    update bullets positions
    for (int i = 0; i < bullets.size(); ++i) bullets[i].y -= 10;

//    remove off-screen bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        if(bullets[i].y < 0) bullets.erase(bullets.begin() + i);
    }

//    render bullets
    for (int i = 0; i < bullets.size(); ++i) SDL_RenderCopy(gRenderer, texture, NULL, &bullets[i]);

    timeStart = currentTime;
}

void Cannon::handleInput(SDL_Event e)
{
    switch (e.key.keysym.sym)
    {

        case SDLK_SPACE:
            break;
        case SDLK_RIGHT:
            position.x += 7;
            if(position.x + position.w > SCREEN_WIDTH) position.x = SCREEN_WIDTH - position.w;
            break;
        case SDLK_LEFT:
            position.x -= 7;
            if(position.x + position.w > SCREEN_WIDTH) position.x = SCREEN_WIDTH - position.w;
            break;
        default:
            break;
    }
}

void Cannon::fire()
{
    SDL_Rect rect;
    rect.x = this->position.x;
    rect.y = this->position.y - position.h;
    rect.h = 50;
    rect.w = 50;
    this->bullets.push_back(rect);
}

#endif // canhao
