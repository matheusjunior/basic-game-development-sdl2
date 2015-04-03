//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include "Util.h"


#ifdef _WIN32

#include <time.h>
#include <Windows.h>
#include <SDL.h>

#else



#endif

int Util::GenerateRandom(int l, int u)
{
    srand(time(NULL));

    double r = rand() % ((u - l) + 1);
    r = l + r;

	return (int) r;
}
