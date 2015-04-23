
#ifdef _WIN32

#include <SDL_timer.h>
#include <SDL_render.h>
#include <SDL_image.h>

#else

#include <SDL2/SDL_timer.h>
#include <iostream>

#endif


#include "GameObject.h"
#include "Consts.h"
#include "Util.h"

GameObject::GameObject(int x, int y, int w, int h)
{
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;
    degree = 0.0f;
    speedX = 20;
    speedY = 10;

    currTexture = NULL;

    timeStart = 0;
    currentTime = 0;
    deltaT = 0;
    imageIndex = 0;
    stopwatch = new Stopwatch();
}

GameObject::GameObject(int x, int y, int w, int h, double speed) : GameObject(x, y, w, h)
{
//    FIXME Lowest moving speed is 25p/s
    speedX = speed;
    speedY = speed;

    currTexture = NULL;
}

GameObject::GameObject(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend) : GameObject(x, y, w, h, vel)
{
	objRend = rend;
	currTexture = getTexture(path);
	isFalling = false;
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
    currTexture = tex;
}

void GameObject::moveX(double dTime)
{
    position.x += (int) speedX * dTime;
}

void GameObject::moveY(double dTime)
{
    position.y += (int) speedY * dTime;
}

void GameObject::Update()
{
    position.x += speedX * (float) (currentTime - timeStart) / 1000;
}

void GameObject::draw(SDL_Renderer *gRenderer)
{
    currentTime = SDL_GetTicks();
    if(currTexture == NULL) exit(9);
    SDL_RenderCopy(gRenderer, currTexture, NULL, &position);
    timeStart = currentTime;
}

void GameObject::draw()
{
	if (currTexture == NULL) return;
	if (isFalling) fall();

	SDL_RenderCopy(objRend, currTexture, NULL, &position);
}

void GameObject::updateSpeedX(double acceleration, double dTime)
{
    speedX += acceleration * dTime;
}

void GameObject::updateSpeedY(double acceleration, double dTime)
{
    speedY += acceleration * dTime;
}

SDL_Texture* GameObject::getTexture(std::string path)
{
	SDL_Surface *surface = SDL_LoadBMP(path.c_str());
	SDL_Texture *text = SDL_CreateTextureFromSurface(objRend, surface);

	return text;
}

void GameObject::stopFalling() {
	isFalling = false;
}

void GameObject::fall() {
	isFalling = true;
}

int GameObject::loadSpriteSheet(std::string path)
{
	SDL_Surface *rect = IMG_Load(path.c_str());

    //SDL_Surface *rect = SDL_LoadBMP(path.c_str());
    if (rect == NULL)
    {
         return -1;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(objRend, rect);
    if (texture == NULL)
    {
         return -1;
    }
    sprites.push_back(texture);
    return 0;
}

void GameObject::show(float dTime) {
//    FPS / # of sprites to get an appropriate time for each sprite
    if ((stopwatch->getCurrTime() - stopwatch->getStartTime()) > (float) 30 / sprites.size())
    {
        stopwatch->stop();
        stopwatch->start();
        currTexture = sprites[++imageIndex % sprites.size()];
        if (imageIndex > 1000)
        {
            imageIndex = 0;
        }
    }
}

void GameObject::rotateRight(SDL_Renderer *gRenderer) {
    
	SDL_Point* pointOfRotation = new SDL_Point();

	pointOfRotation->x = position.w / 2;
	pointOfRotation->y = (position.h / 3) + (position.h / 2);
	
	SDL_RenderCopyEx(gRenderer, currTexture, NULL, &position, degree, pointOfRotation, SDL_FLIP_NONE);
}

void GameObject::rotateLeft(SDL_Renderer *gRenderer) {
	
	SDL_Point* pointOfRotation = new SDL_Point();
	
	pointOfRotation->x = position.w / 2;
	pointOfRotation->y = (position.h / 3) + (position.h / 2);




    SDL_RenderCopyEx(gRenderer, currTexture, NULL, &position, degree, pointOfRotation, SDL_FLIP_NONE);
}