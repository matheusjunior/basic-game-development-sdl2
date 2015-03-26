// TODO Properly separate Cannon.cpp and Cannon.h
//#include <SDL2/SDL_timer.h>
//#include "Cannon.h"
//
//void Cannon::draw(SDL_Renderer *gRenderer)
//{
//    currentTime = SDL_GetTicks();
//    //Update();
//    if(texture == NULL) exit(9);
//    SDL_RenderCopy(gRenderer, texture, NULL, &position);
//
////    update bullets positions
//    for (int i = 0; i < bullets.size(); ++i) bullets[i].y -= 10;
//
////    remove off-screen bullets
//    for (int i = 0; i < bullets.size(); ++i)
//    {
//        if(bullets[i].y < 0) bullets.erase(bullets.begin() + i);
//    }
//
////    render bullets
//    for (int i = 0; i < bullets.size(); ++i) SDL_RenderCopy(gRenderer, texture, NULL, &bullets[i]);
//
//    timeStart = currentTime;
//}
//
//
//void Cannon::fire()
//{
//    SDL_Rect rect;
//    rect.x = this->position.x;
//    rect.y = this->position.y - position.h;
//    rect.h = 50;
//    rect.w = 50;
//    this->bullets.push_back(rect);
//}