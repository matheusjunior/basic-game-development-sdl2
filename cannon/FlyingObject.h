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
#include "Vector2d.h"

#include <list>
#include <vector>


#include <iostream>

using namespace std;


/* TODO Document class
* */
class FlyingObject : public GameObject
{
private:
	bool isFalling;
	double deltaTime;
	Vector2d vecDir;


public:
	void setDeltaTime(double dt);

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

	void loadBaseImage();

	void fire(SDL_Rect rect);

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

void FlyingObject::setDeltaTime(double dt) {
	deltaTime = dt;
}

void FlyingObject::draw()
{
	//Update();
	if (currTexture == NULL) exit(9);
	SDL_RenderCopy(objRend, currTexture, NULL, &position);

	//    update bullets positions
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].updateSpeedX(5 * vecDir.x);
		bullets[i].updateSpeedY(5 * vecDir.y);
		bullets[i].moveX(0.1);
		bullets[i].moveY(0.1);
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



void FlyingObject::loadBaseImage(){
	SDL_RenderCopy(objRend, currTexture, NULL, &position);
}

void FlyingObject::fire(SDL_Rect cannon) {

	Vector2d vFlying;
	Vector2d vCannon;

	vFlying.x = position.x + position.w / 2;
	vFlying.y = position.y + position.h / 2;

	vCannon.x = cannon.x + cannon.w / 2;
	vCannon.y = cannon.y + cannon.h / 2;

	vecDir = Util::getDistance(vFlying, vCannon);

	double length = sqrt(vecDir.x*vecDir.x + vecDir.y*vecDir.y);

	//normalize

	vecDir.x = vecDir.x / length;
	vecDir.y = vecDir.y / length;

	int teste = 0;

	if ((vFlying.x + position.w / 2) > vCannon.x) {
		vecDir.x = -vecDir.x;
	}

	GameObject bullet;
	bullet.position.x = position.x + position.w / 2;
	bullet.position.y = position.y + 15;
	bullet.position.h = 10;
	bullet.position.w = 10;
	bullet.speedX = 0;
	bullet.speedY = 0;

	this->bullets.push_back(bullet);
}

void FlyingObject::fire(){}


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

	//cout << speedY << endl;
}



#endif
