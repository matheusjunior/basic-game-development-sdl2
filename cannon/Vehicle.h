#ifndef vehicle
#define vehicle

#ifdef _WIN32

#include <SDL_render.h>
#include <SDL_events.h>

#else

#import <SDL2/SDL_render.h>
#import <SDL2/SDL_events.h>

#endif

#include "Vector2d.h"

class Vehicle {
private:
	Vector2d _Position;
	Vector2d _Velocity;
	float    _Mass;


public:

	void Update(float TimeElapsedSinceLastUpdate) {
		_Position += _Velocity * TimeElapsedSinceLastUpdate;
	}

	void Update(float TimeElapsedSinceLastUpdate, float ForceOnShip) {
		float acceleration = ForceOnShip / _Mass;

	}

};

#endif