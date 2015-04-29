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


/** Represents a cannon object in the game
* */
class Cannon : public GameObject
{
public:
    enum LIFE_STATE {STRONG, WEAK, DEAD};

private:
    int lifes;
    LIFE_STATE lifeState;

public:
    /// Bullets for the cannon
    std::vector<GameObject> bullets;
	SDL_Texture *bulletTexture;

    Cannon(int x, int y, int w, int h, double vel);

	Cannon(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet);


    LIFE_STATE const &getLifeState() const
    {
        return lifeState;
    }

    void setLifeState(LIFE_STATE const &lifeState)
    {
        Cannon::lifeState = lifeState;
    }

    int getLifes() const
    {
        return lifes;
    }

    void setLifes(int lifes)
    {
        Cannon::lifes = lifes;
//        Very simple state machine. It changes cannon shooting behavior
        if (this->lifes == 1) lifeState = WEAK;
        else if (this->lifes == 0) lifeState = DEAD;
    }

/** Draw GameObject including the bullets shot
    * \param gRenderer SDL_Render to apply the currTexture
    * */
    void draw(SDL_Renderer *gRenderer) override;

	SDL_Texture* getTexture(SDL_Renderer *rend, std::string path);

    /** Fire bullets (squares for now)
    * */
    void fire();

    void drawBullets(SDL_Renderer *pRenderer);

	void loadBaseImage();
};

Cannon::Cannon(int x, int y, int w, int h, double vel) : GameObject(x, y, w, h, vel)
{
}

Cannon::Cannon(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend, std::string pathBullet) : GameObject(x, y, w, h, vel, path, rend)
{
	bulletTexture = getTexture(rend, pathBullet);
	currTexture = getTexture(rend, path);
    lifes = 2;
    lifeState = STRONG;
//	currTexture = currTexture;
}

SDL_Texture* Cannon::getTexture(SDL_Renderer *rend, std::string path) {
	SDL_Surface *surface = IMG_Load(path.c_str());

	SDL_Texture *text = SDL_CreateTextureFromSurface(rend, surface);

	return text;
}

void Cannon::draw(SDL_Renderer *gRenderer)
{
    if(currTexture == NULL) exit(9);
    SDL_RenderCopy(gRenderer, currTexture, NULL, &position);

    drawBullets(gRenderer);
}

void Cannon::loadBaseImage(){	

	SDL_RenderCopy(objRend, currTexture, NULL, &position);
}

void Cannon::drawBullets(SDL_Renderer *renderer)
{
    //    update bullets positions
    for (size_t i = 0; i < bullets.size(); i++) bullets[i].position.y -= 10;

//    remove off-screen bullets
    for (size_t i = 0; i < bullets.size(); i++)
    {
        if(bullets[i].position.y < 0) bullets.erase(bullets.begin() + i);
    }

//    render bullets
    for (size_t i = 0; i < bullets.size(); i++) SDL_RenderCopy(renderer, bulletTexture, NULL, &bullets[i].position);
}

void Cannon::fire()
{
    GameObject bullet;
    bullet.position.x = this->position.x + 15;
    bullet.position.y = this->position.y;

//    Change shooting behavior
    if (lifeState == STRONG)
    {
        bullet.position.h = 10;
        bullet.position.w = 10;
    }
    else if (lifeState == WEAK || lifeState == DEAD)
    {
        bullet.position.h = 5;
        bullet.position.w = 5;
    }

    this->bullets.push_back(bullet);
    
    //nao tenho certeza se compila, mas sei que tem que executar essa linha
    bullet.speedX = speedX;
}

#endif 
