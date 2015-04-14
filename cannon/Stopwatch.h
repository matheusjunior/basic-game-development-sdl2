//
// Created by Matheus Pereira Junior on 4/14/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __Timer_H_
#define __Timer_H_


#include <_types/_uint32_t.h>

class Stopwatch
{
private:
    enum State {RUNNING, PAUSED, STOPPED};
    State state;
    uint32_t startTime;
    uint32_t pauseTime;
    uint32_t stopTime;

public:
    Stopwatch();

    State const &getState() const
    {
        return state;
    }

    uint32_t getStartTime() const
    {
        return startTime;
    }

    void setStartTime(uint32_t startTime)
    {
        Stopwatch::startTime = startTime;
    }

    uint32_t getPauseTime() const
    {
        return pauseTime;
    }

    void setPauseTime(uint32_t pauseTime)
    {
        Stopwatch::pauseTime = pauseTime;
    }

    uint32_t getStopTime() const
    {
        return stopTime;
    }

    void setStopTime(uint32_t stopTime)
    {
        Stopwatch::stopTime = stopTime;
    }

    void start();
    void pause();
    void stop();
    uint32_t getCurrTime();
};


#endif //__Timer_H_