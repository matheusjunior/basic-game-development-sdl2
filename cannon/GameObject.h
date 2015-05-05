#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#ifdef _WIN32

#include <SDL_render.h>
#include <SDL_events.h>

#else

#import <SDL2/SDL_render.h>
#import <SDL2/SDL_events.h>

#endif

#include <string>
#include <vector>

#include "Stopwatch.h"
#include "Consts.h"

/*! \brief Represents all possible movable objects
*
* \todo Document functions
* */;
class GameObject
{
private:
	bool isFalling;
	int imageIndex;
	Stopwatch *stopwatch;

public:
	double degree = 90;

	double getDegree() const {
		return degree;
	}

	bool updateDegree(double degree) {
		if (GameObject::degree + degree < 60 && GameObject::degree + degree > -60) {
			GameObject::degree += degree;
			return true;
		}
		return false;
	}

	void setDegree(double degree) {
		
		GameObject::degree = degree;
	}

	Stopwatch *getStopwatch() const {
		return stopwatch;
	}

	void setStopwatch(Stopwatch *stopwatch) {
		GameObject::stopwatch = stopwatch;
	}

	/// GameObject Texture
	SDL_Texture *currTexture;
	/// Sprite sheet
	std::vector<SDL_Texture*> sprites;
	/// GameObject position in space
	SDL_Rect position;
	SDL_Renderer *objRend;
	/// x axis speed in pixels/sec
	double speedX;
	/// y axis speed in pixels/sec
	double speedY;
	uint32_t timeStart;
	uint32_t deltaT;
	uint32_t currentTime;

	GameObject() {}

	GameObject(int x, int y, int w, int h);

	GameObject(int x, int y, int w, int h, double speed);

	GameObject(int x, int y, int w, int h, double vel, std::string path, SDL_Renderer *rend);

	/**
	* Move object along x axis
	* \param y represents the elapsed time in ms to move GameObject according to its speed given in pixels/sec
	* */
	void moveX(double dTime);

	/**
	* Move object along y axis
	* \param y represents the elapsed time in ms to move GameObject according to its speed given in pixels/sec
	* */
	void moveY(double dTime);


	/**
	* Change GameObject axis x speed
	* \param v new speed in pixels/sec
	* */
	void setSpeedX(double v);

	/** Change GameObject axis x speed
	* \param v new speed in pixels/sec
	* */
	void setSpeedY(double v);

	void updateSpeedX(double acceleration);

	void updateSpeedY(double acceleration);

	void updateSpeedX(double acceleration, double dTime);

	void updateSpeedY(double acceleration, double dTime);

	int loadSpriteSheet(std::string path);

	void show(float dTime);

	/** Set GameObject currTexture
	* \param tex currTexture to be applied
	* */
	void setTexture(SDL_Texture *tex);

	/** Draw GameObject using its own currTexture
	* \param gRenderer SDL_Render to apply the currTexture
	* */
	virtual void draw(SDL_Renderer *gRenderer);

	virtual void draw();

	/* TODO Document function
	* */
	void Update();

	//private:
	// SDL_Surface surface;

	SDL_Texture *getTexture(std::string path);

	void fall();

	void stopFalling();

	/** Rotate right and paint object on screen
	* \param gRenderer Render object
	*/
	void rotateRight(SDL_Renderer *gRenderer);

	/** Rotate left and paint object on screen
	* \param gRenderer Render object
	* \fixme Not really neeed. SDL_RenderCopyEx takes cares of checking the angle, so we just need to increase/decrease the angle
	*
	*/
	void rotateLeft(SDL_Renderer *gRenderer);
};

#endif // objeto
