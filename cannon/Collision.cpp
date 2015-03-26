//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include "Collision.h"

bool Collision::AABBCollision(SDL_Rect *a, SDL_Rect *b)
{
    bool xCollision;
    bool yCollision;

    if (a->x > b->x + b->w || b->x > a->x + a->w) xCollision = false;
    else xCollision = true;

    if (a->y - a->h > b->y + b->h || b->y - b->h > a->y + a->h) yCollision = false;
    else yCollision = true;

    if (xCollision && yCollision) return true;
    return false;
}
