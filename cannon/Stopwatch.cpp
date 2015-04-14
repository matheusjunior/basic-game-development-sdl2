//
// Created by Matheus Pereira Junior on 4/14/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include <SDL2/SDL_timer.h>
#include "Stopwatch.h"

void Stopwatch::start()
{
///   \todo enum not working
    state = State::RUNNING;
    startTime = SDL_GetTicks();
}

void Stopwatch::pause()
{
    state = State::PAUSED;
    pauseTime = SDL_GetTicks();
}

void Stopwatch::stop()
{
    state = State::STOPPED;
    stopTime = SDL_GetTicks();
}

Stopwatch::Stopwatch()
{
    state = State::STOPPED;
    startTime = 0;
    pauseTime = 0;
    stopTime = 0;
}

uint32_t Stopwatch::getCurrTime()
{
    return SDL_GetTicks();
}