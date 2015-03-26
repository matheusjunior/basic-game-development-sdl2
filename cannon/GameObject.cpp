#include <SDL2/SDL_timer.h>
#include "GameObject.h"
#include "Consts.h"

GameObject::GameObject(int x, int y, int w, int h)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

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

void GameObject::moveX(double x)
{
    position.x += speedX * ((float) x / 1000);
}

void GameObject::moveY(double y)
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
