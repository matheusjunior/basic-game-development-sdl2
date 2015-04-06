#include <SDL2/SDL_timer.h>
#include "GameObject.h"
#include "Consts.h"

GameObject::GameObject(int x, int y, int w, int h)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    speedX = 20;
    speedY = 10;

    texture = NULL;

    timeStart = 0;
    currentTime = 0;
    deltaT = 0;
}

GameObject::GameObject(int x, int y, int w, int h, double speed)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;
//    FIXME Lowest moving speed is 25p/s
    speedX = speed;
    speedY = speed;

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

void GameObject::moveX(double dTime)
{
    position.x += speedX * dTime;
}

void GameObject::moveY(double dTime)
{
    position.y += speedY * dTime;
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

void GameObject::updateSpeedX(double acceleration, double dTime)
{
    speedX += acceleration * dTime;
}

void GameObject::updateSpeedY(double acceleration, double dTime)
{
    speedY += acceleration * dTime;
}
