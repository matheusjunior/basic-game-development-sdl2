#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#import <SDL2/SDL_render.h>
#import <SDL2/SDL_events.h>
#import <SDL2/SDL_timer.h>
#include "Consts.h"

class GameObject
{
public:
    SDL_Texture *texture;
    SDL_Rect position;
    double speedX;
    double speedY;
    double timeStart;
    double currentTime;
    double deltaT;

    GameObject()
    {
    }

    GameObject(int x, int y, int w, int h);

    GameObject(int x, int y, int w, int h, double vel);

    void moverX(double x);

    void moverY(double y);

    void setSpeedX(double v);

    void setSpeedY(double v);

    void setTexture(SDL_Texture *tex);

    virtual void draw(SDL_Renderer *gRenderer);

    void Update();

    void handleInput(SDL_Event e);
//private:
    // SDL_Surface surface;
};


GameObject::GameObject(int x, int y, int w, int h)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

//    pixels/sec
    speedX = 400;
    speedY = 100;

    texture = NULL;

    timeStart = 0;
    currentTime = 0;
    deltaT = 0;
}

GameObject::GameObject(int x, int y, int w, int h, double vel)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    speedX = vel;
    speedY = vel;

    texture = NULL;

    timeStart = 0;
    currentTime = 0;
    deltaT = 0;
}


void GameObject::setSpeedX(double v)
{
    speedX = v;
}

void GameObject::setSpeedY(double v)
{
    speedY = v;
}

void GameObject::setTexture(SDL_Texture *tex)
{
    texture = tex;
}

void GameObject::moverX(double x)
{
    position.x += speedX * ((float) x / 1000);
}

void GameObject::moverY(double y)
{
    position.y += speedY * y;
}

void GameObject::Update()
{
    position.x += speedX * (float) (currentTime - timeStart) / 1000;
}

void GameObject::draw(SDL_Renderer *gRenderer)
{
    currentTime = SDL_GetTicks();
    //Update();
    if(NULL == texture) exit(9);
    SDL_RenderCopy(gRenderer, texture, NULL, &position);
    timeStart = currentTime;
}

void GameObject::handleInput(SDL_Event e)
{

    switch (e.key.keysym.sym)
    {
        case SDLK_UP:
            position.y -= 5;
            if(position.y < 0) position.y = 0;
            break;
        case SDLK_DOWN:
            position.y += 5;
            if(position.y + position.h > SCREEN_HEIGHT) position.y = SCREEN_HEIGHT - position.h;
            break;
        case SDLK_LEFT:
            position.x -= 5;
            if(position.x < 0)
            {
                position.x = 0;
            }
            break;
        case SDLK_RIGHT:
            position.x += 5;
            if(position.x + position.w > SCREEN_WIDTH) position.x = SCREEN_WIDTH - position.w;
            break;
        default:
            break;
    }
}

#endif // objeto
