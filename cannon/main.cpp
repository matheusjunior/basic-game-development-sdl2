#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>

#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>

#endif

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "GameObject.h"
#include "Cannon.h"
#include "Text.h"
#include "Collision.h"
#include "Util.h"
#include "FlyingObject.h"
#include "MusicPlayer.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
Mix_Music* music = NULL;
MusicPlayer* musicPlayer = NULL;

using namespace std;

/** Initialize SDL components
* \return true if: video component initialized properly, window succesfully created,
* and SDL_Tff is properly initialized
* false otherwise
* */
bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

	gWindow = SDL_CreateWindow("Cannon Game !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (gWindow == NULL) return false;
	if (TTF_Init() == -1)
	{
		std::cout << "Error while initializing SDL_Ttf: %s" << TTF_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG) == 0)
	{
		std::cout << "Error while initializing SDL_Ttf: %s" << IMG_GetError() << endl;
		return false;
	}

	if (Mix_OpenAudio(musicPlayer->getFreq(), MIX_DEFAULT_FORMAT,
		musicPlayer->getQuantChannels(), musicPlayer->getChunksize()) == NULL) cout << Mix_GetError() << endl;

	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	return true;
}

/** Close SDL resources in use
* */
void close()
{
	SDL_DestroyWindow(gWindow);
	Mix_FreeMusic(music);
	gWindow = NULL;

	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

/** Game main function. All event handling is first processed here
* and delegated to classes if needed.
* \return 0 if succeeds, -1 otherwise
* */
int main(int argc, char *args[])
{
	srand(time(NULL));

	const Uint8 *currKeyStates;
	SDL_Event e;
	SDL_Surface *backgroundSurf = NULL;
	SDL_Texture *backgroudText = NULL;
	Text text;
	Text textFPS;
	uint32_t startFrameTime = 0;
	uint32_t endFrameTime = 0;
	bool quit = false;

	float startMoveTime = 0;
	float endMoveTime = 0;
	float dMoveTime = 0;
	float deltaTime;

	int lastFrameTime = 0, currentFrameTime = 0;
	int fps = 40;
	int fpsMill = 1000 / fps;
	int totalFrames = fps;
	int kills = 0;
	int currentSpeed = fps;

	enum Rotation { ROTATE_LEFT, ROTATE_RIGHT, NO_ROTATION };
	Rotation rotation = NO_ROTATION;

	musicPlayer = new MusicPlayer();

	if (!init())
	{
		std::cout << "Falhou init\n";
		return -1;
	}

	Cannon* cannon = new Cannon(SCREEN_WIDTH / 2 - 40 , SCREEN_WIDTH / 2 + 30 , 80, 105, 400,
		"media/cannon.bmp", gRenderer, "media/red-square.bmp");
	cannon->loadSpriteSheet("media/c1.png");
	cannon->loadSpriteSheet("media/c2.png");
	cannon->loadSpriteSheet("media/c3.png");
	cannon->loadSpriteSheet("media/c4.png");
	cannon->loadSpriteSheet("media/c5.png");
	cannon->loadSpriteSheet("media/c6.png");
	cannon->loadSpriteSheet("media/c7.png");
	cannon->loadSpriteSheet("media/c8.png");
	cannon->loadSpriteSheet("media/c9.png");
	cannon->loadBaseImage();
	cannon->getStopwatch()->start();

	int px = -25 + Util::GenerateRandom(0, 60);
	int py = Util::GenerateRandom(0, 70);
	int speed = Util::GenerateRandom(100, 200);
	FlyingObject fly1(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	px = -25 + Util::GenerateRandom(1, 61);
	py = Util::GenerateRandom(60, 140);
	speed = Util::GenerateRandom(100, 200);
	FlyingObject fly2(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	px = -25 + Util::GenerateRandom(2, 62);
	py = Util::GenerateRandom(120, 210);
	speed = Util::GenerateRandom(100, 200);
	FlyingObject fly3(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	px = -55 + Util::GenerateRandom(0, 60);
	py = Util::GenerateRandom(200, 280);
	speed = Util::GenerateRandom(100, 00);
	FlyingObject fly4(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	std::vector<FlyingObject> ovnis;
	fly1.speedY = 0;
	fly2.speedY = 0;
	fly3.speedY = 0;
	fly4.speedY = 0;
	ovnis.push_back(fly1);
	ovnis.push_back(fly2);
	ovnis.push_back(fly3);
	ovnis.push_back(fly4);

	FlyingObject point(px, py, 25, 25, 0, "media/green1-square.bmp", gRenderer, "media/red-square.bmp");

	text.font = TTF_OpenFont("media/emulogic.ttf", 20);
	textFPS.font = TTF_OpenFont("media/emulogic.ttf", 20);

	textFPS.rect.x = 220;
	textFPS.rect.y = 10;
	textFPS.rect.w = 600;
	textFPS.rect.h = 25;

	if (text.font == NULL || textFPS.font == NULL)
	{
		std::cout << "Error: " << TTF_GetError() << endl;
		return -1;
	}

	backgroundSurf = IMG_Load("media/desert.jpg");
	backgroudText = SDL_CreateTextureFromSurface(gRenderer, backgroundSurf);

	//    hope the error is saved
	musicPlayer->createPlayList();
	musicPlayer->playCurrPlaylist();

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) quit = true;
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE: 
					quit = true;
					break;
				case SDLK_SPACE:
					cannon->fire();
					break;
				case SDLK_RIGHT:
					cannon->position.x += 7;
					if (cannon->position.x + cannon->position.w > SCREEN_WIDTH) cannon->position.x = SCREEN_WIDTH - cannon->position.w;
					break;
				case SDLK_LEFT:
					cannon->position.x -= 7;
					if (cannon->position.x + cannon->position.w > SCREEN_WIDTH) cannon->position.x = SCREEN_WIDTH - cannon->position.w;
					break;
				case SDLK_r:
					cannon->setDegree(cannon->getDegree() + 5);
					rotation = ROTATE_RIGHT;
					break;
				case SDLK_e:
					cannon->setDegree(cannon->getDegree() - 5);
					rotation = ROTATE_LEFT;
					break;
				default:
					break;
				}
			}
		}
		//        \todo Keyboard states read not working
		//        currKeyStates = SDL_GetKeyboardState(NULL);

		//        if (currKeyStates[SDL_SCANCODE_LEFT]) cannon->position.x += 7;
		//        if (currKeyStates[SDL_SCANCODE_RIGHT]) cannon->moveX(10);
		//        if (currKeyStates[SDL_SCANCODE_UP]) cannon->moveY(-10);
		//        if (currKeyStates[SDL_SCANCODE_DOWN]) cannon->moveY(10);
		//        if (currKeyStates[SDL_SCANCODE_SPACE]) cannon->fire();
		//
		//        if (currKeyStates[SDL_SCANCODE_U]) musicPlayer->decreaseVolume();
		//        if (currKeyStates[SDL_SCANCODE_I]) musicPlayer->increaseVolume();
		//        if (currKeyStates[SDL_SCANCODE_B]) musicPlayer->previousMusic();
		//        if (currKeyStates[SDL_SCANCODE_N]) musicPlayer->nextMusic();


		startFrameTime = SDL_GetTicks();
		totalFrames++;
		SDL_RenderClear(gRenderer);
		//        Background image should be the first one to be rendered, otherwise overwrites renders
		SDL_RenderCopy(gRenderer, backgroudText, NULL, NULL);

		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
		lastFrameTime = SDL_GetTicks();

		for (size_t i = 0; i < ovnis.size(); i++) {
			int randomShot;

			if (ovnis[i].position.x > SCREEN_WIDTH) {
				ovnis[i].position.x = 0;
				ovnis[i].position.y = Util::GenerateRandom(0, SCREEN_HEIGHT / 4) + Util::GenerateRandom(0, SCREEN_HEIGHT / 4);
				ovnis[i].stopFalling();
			}
			else if (ovnis[i].isIsFalling())
			{
				//                FIXME check why 99 works and 9.8 doesn't
				ovnis[i].updateSpeedX(0, deltaTime);
				ovnis[i].updateSpeedY(99, deltaTime);
				ovnis[i].moveX(deltaTime);
				ovnis[i].moveY(deltaTime);
			}
			else {
				randomShot = Util::GenerateRandom(0, 100);
				//cout << randomShot << endl;
				ovnis[i].moveX(deltaTime);
				if (randomShot > 99) { // 1% de chance de atirar
					ovnis[i].setDeltaTime(deltaTime);

					ovnis[i].fire(cannon->position);
				}

			}
			ovnis[i].setDT(deltaTime);
			ovnis[i].draw();
		}
		cannon->show(0.0f);

		if (rotation == ROTATE_LEFT) cannon->rotateLeft(gRenderer);
		else if (rotation == ROTATE_RIGHT) cannon->rotateRight(gRenderer);
		else cannon->draw(gRenderer);

		cannon->drawBullets(gRenderer);
		for (size_t i = 0; i < cannon->bullets.size(); i++)
		{
			for (size_t j = 0; j < ovnis.size(); j++) {
				if (Collision::CircleCollision(ovnis[j].position, cannon->bullets[i].position)) {
					ovnis[j].fall();
					kills++;
					cannon->bullets.at(i).position.y = 0;
					musicPlayer->playSoundEffect("media/effect.wav");
				}
				if (Collision::AABBCollision(&ovnis[i].position, &cannon->bullets[i].position))
				{
					kills++;
					musicPlayer->playSoundEffect("media/effect.wav");
					break; // cannot kill twice in a row
				}
			}
		}
		if (kills >= 1000) kills = 0;
		std::stringstream temp;
		temp << kills;

		//        FIXME Poor performance
		text.displayText = "Kills: " + temp.str();

		std::stringstream deltaString;
		deltaString << deltaTime;

		std::stringstream waitTime;
		waitTime << fpsMill;

		std::stringstream fspString;
		fspString << fps;

		textFPS.displayText = "DELTA: " + deltaString.str() + "  WAIT: " + waitTime.str() + "\n  FPS: " + fspString.str();

		text.surface = TTF_RenderText_Solid(text.font, text.displayText.c_str(), text.color);
		textFPS.surface = TTF_RenderText_Solid(textFPS.font, textFPS.displayText.c_str(), textFPS.color);

		if (text.surface == NULL || textFPS.surface == NULL)
		{
			std::cout << "Error:" << TTF_GetError() << endl;
			return -1;
		}

		text.texture = SDL_CreateTextureFromSurface(gRenderer, text.surface);
		textFPS.texture = SDL_CreateTextureFromSurface(gRenderer, textFPS.surface);


		cannon->show(0.0f);

		if (rotation == ROTATE_LEFT) cannon->rotateLeft(gRenderer);
		else if (rotation == ROTATE_RIGHT) cannon->rotateRight(gRenderer);
		else cannon->draw(gRenderer);

		SDL_RenderCopy(gRenderer, text.texture, NULL, &text.rect);
		SDL_RenderCopy(gRenderer, textFPS.texture, NULL, &textFPS.rect);

		point.position.h = 3;
		point.position.w = 3;
		point.position.x = cannon->position.x + cannon->position.w / 2;
		point.position.y = cannon->position.y + cannon->position.h / 2;

		point.draw();
		
		

		SDL_RenderPresent(gRenderer);
		//std::cout << fpsMill << " dt " << deltaTime << endl;

		SDL_Delay(fpsMill - deltaTime);
	}
	close();
	return 0;
}
