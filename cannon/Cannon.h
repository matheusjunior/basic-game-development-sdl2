#ifndef CANNON_H_
#define CANNON_H_

#import <SDL2/SDL_events.h>
#import "GameObject.h"
#include "Consts.h"
#include <list>
#include <vector>

/* TODO Document class
* */
class Cannon : public GameObject
{
public:
    /* Bullets for the cannon
    * TODO Implement bullets as a class
    * */
    std::vector<GameObject> bullets;
public:
    Cannon(int x, int y, int w, int h, double vel);

    /* Draw GameObject including the bullets shot
    * @param gRenderer SDL_Render to apply the texture
    * */
    void draw(SDL_Renderer *gRenderer) override;

    /* Fire bullets (squares for now)
    * */
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
    for (int i = 0; i < bullets.size(); ++i) bullets[i].position.y -= 10;

//    remove off-screen bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        if(bullets[i].position.y < 0) bullets.erase(bullets.begin() + i);
    }

//    render bullets
    for (int i = 0; i < bullets.size(); ++i) SDL_RenderCopy(gRenderer, texture, NULL, &bullets[i].position);

    timeStart = currentTime;
}

void Cannon::fire()
{
    GameObject bullet;
    bullet.position.x = this->position.x;
    bullet.position.y = this->position.y - position.h;
    bullet.position.h = 50;
    bullet.position.w = 50;
    this->bullets.push_back(bullet);
}

#endif // canhao
