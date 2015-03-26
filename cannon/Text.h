//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __Text_H_
#define __Text_H_


#include <SDL2/SDL_render.h>
#include <SDL2_ttf/SDL_ttf.h>

/* TODO Create interface for applying texture
* TODO Document class
* */
class Text
{
public:
    SDL_Surface *surface;
    SDL_Rect rect;
    TTF_Font *font;
    SDL_Color color = {255, 0, 0};
    SDL_Texture *texture;

    Text();
};


#endif //__Text_H_
