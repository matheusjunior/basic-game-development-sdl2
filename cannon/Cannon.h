#ifndef CANNON_H_
#define CANNON_H_


#ifdef _WIN32

#include <SDL.h>
#include <SDL_events.h>

#else

#include <SDL2/SDL.h>
#import <SDL2/SDL_events.h>

#endif


#include "GameObject.h"
#include "Consts.h"
#include <list>
#include <vector>
#include <iostream>

using namespace std;

/* TODO Document class
* */
class Cannon : public GameObject
{
public:
    /* Bullets for the cannon
    * TODO Implement bullets as a class
    * */
    std::vector<GameObject> bullets;
	SDL_Texture *bulletTexture;

public:
    Cannon(int x, int y, int w, int h, double vel);

	Cannon(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet);

    /* Draw GameObject including the bullets shot
    * @param gRenderer SDL_Render to apply the texture
    * */
    void draw(SDL_Renderer *gRenderer) override;

	SDL_Texture* getTexture(SDL_Renderer *rend, std::string path);

    /* Fire bullets (squares for now)
    * */
    void fire();
};

Cannon::Cannon(int x, int y, int w, int h, double vel) : GameObject(x, y, w, h, vel)
{
}

Cannon::Cannon(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet) : GameObject(x, y, w, h, vel, path, rend)
{
	bulletTexture = getTexture(rend, pathBullet);
}

SDL_Texture* Cannon::getTexture(SDL_Renderer *rend, std::string path) {
	SDL_Surface *surface = SDL_LoadBMP(path.c_str());

	SDL_Texture *text = SDL_CreateTextureFromSurface(rend, surface);

	return text;
}

void Cannon::draw(SDL_Renderer *gRenderer)
{
    currentTime = SDL_GetTicks();
    //Update();
    if(texture == NULL) exit(9);
    SDL_RenderCopy(gRenderer, texture, NULL, &position);

//    update bullets positions
    for (int i = 0; i < bullets.size(); ++i) bullets[i].position.y -= 10;

//    remove off-screen bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        if(bullets[i].position.y < 0) bullets.erase(bullets.begin() + i);
    }

//    render bullets

	if (bulletTexture == NULL) cout << "null";
    for (int i = 0; i < bullets.size(); ++i) SDL_RenderCopy(gRenderer, bulletTexture, NULL, &bullets[i].position);

    timeStart = currentTime;
}

void Cannon::fire()
{
    GameObject bullet;
    bullet.position.x = this->position.x  + 40;
    bullet.position.y = this->position.y - 20;
    bullet.position.h = 30;
    bullet.position.w = 30;
    this->bullets.push_back(bullet);
}

#endif // canhao
