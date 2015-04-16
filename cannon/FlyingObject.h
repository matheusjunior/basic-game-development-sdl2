#ifndef FLYING_OBJ_H_
#define FLYING_OBJ_H_


#ifdef _WIN32

#include <SDL.h>
#include <SDL_events.h>

#else

#include <SDL2/SDL.h>
#import <SDL2/SDL_events.h>

#endif


#include "GameObject.h"
#include "Consts.h"
#include "Util.h"
#include <list>
#include <vector>


#include <iostream>


/* TODO Document class
* */
class FlyingObject : public GameObject
{
private:
    bool isFalling;
	double deltaTime;


public:
    bool isIsFalling() const
    {
        return isFalling;
    }

    void setIsFalling(bool isFalling)
    {
        FlyingObject::isFalling = isFalling;
    }

    /* Bullets for the cannon
    * TODO Implement bullets as a class
    * */
    std::vector<GameObject> bullets;
	SDL_Texture *bulletTexture;

    FlyingObject(int x, int y, int w, int h, double vel);

	FlyingObject(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet);

	void updateSpeedX(double acceleration, double dTime);

	void updateSpeedY(double acceleration, double dTime);

	void draw() override;

	SDL_Texture* getTexture(SDL_Renderer *rend, std::string path);

    /* Fire bullets (squares for now)
    * */
    void fire();

	void setTextureBody(SDL_Texture *t);

	void setTextureBullet(SDL_Texture *t);

	void stopFalling();

	void fall();

	void setDT(double dt) { deltaTime = dt; }

};

FlyingObject::FlyingObject(int x, int y, int w, int h, double vel) : GameObject(x, y, w, h, vel)
{
}

FlyingObject::FlyingObject(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet) : GameObject(x, y, w, h, vel, path, rend)
{
	bulletTexture = getTexture(rend, pathBullet);
}

SDL_Texture* FlyingObject::getTexture(SDL_Renderer *rend, std::string path) {
	SDL_Surface *surface = SDL_LoadBMP(path.c_str());

	SDL_Texture *text = SDL_CreateTextureFromSurface(rend, surface);

	return text;
}


void FlyingObject::draw()
{
	//Update();
	if (currTexture == NULL) exit(9);
	SDL_RenderCopy(objRend, currTexture, NULL, &position);

	//    update bullets positions
	for (size_t i = 0; i < bullets.size(); i++) {
		std::cout << speedX;
		bullets[i].updateSpeedX(0, deltaTime);
		bullets[i].updateSpeedY(75, deltaTime);
		bullets[i].moveX(deltaTime);
		bullets[i].moveY(deltaTime);
	}

	//    remove off-screen bullets
	for (size_t i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].position.y < 0) bullets.erase(bullets.begin() + i);
	}

	//    render bullets
	for (size_t i = 0; i < bullets.size(); i++) SDL_RenderCopy(objRend, bulletTexture, NULL, &bullets[i].position);

	if (isFalling) {
		fall();
	}
}


void FlyingObject::fire()
{
    GameObject bullet;
	bullet.position.x = this->position.x;
	bullet.position.y = this->position.y + 5;
    bullet.position.h = 10;
    bullet.position.w = 10;
    this->bullets.push_back(bullet);

	bullet.speedX = this->speedX;
}


void FlyingObject::setTextureBody(SDL_Texture *t){
	currTexture = t;
}

void FlyingObject::setTextureBullet(SDL_Texture *t) {
	bulletTexture = t;
}

void FlyingObject::stopFalling() {
	isFalling = false;
}

void FlyingObject::fall() {
	isFalling = true;
}

void FlyingObject::updateSpeedX(double acceleration, double dTime)
{
	deltaT = dTime;
	speedX += acceleration * dTime;
}

void FlyingObject::updateSpeedY(double acceleration, double dTime)
{
	deltaT = dTime;
	speedY += acceleration * dTime;
}



#endif
