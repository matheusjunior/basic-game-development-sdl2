
#ifdef _WIN32

#include <SDL_timer.h>
#include <SDL_render.h>

#else

#include <SDL2/SDL_timer.h>

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

GameObject::GameObject(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend)
{
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;

	speedX = vel;
	speedY = vel;

	objRend = rend;

	texture = getTexture(path);

	timeStart = 0;
	currentTime = 0;
	deltaT = 0;

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
    if(NULL == texture) return;
    SDL_RenderCopy(gRenderer, texture, NULL, &position);
    timeStart = currentTime;
}

void GameObject::draw()
{
	currentTime = SDL_GetTicks();
	//Update();
	if (NULL == texture) return;

	if (isFalling == true) fall();

	SDL_RenderCopy(objRend, texture, NULL, &position);
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
	position.y += Util::GenerateRandom(2,5);
	position.x += Util::GenerateRandom(0, 1);
	isFalling = true;
}