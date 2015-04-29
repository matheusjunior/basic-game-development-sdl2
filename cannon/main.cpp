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
//	const Uint8 *currKeyStates;
	SDL_Event e;
	SDL_Surface *backgroundSurf = NULL;
	SDL_Texture *backgroundText = NULL;
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT + 10};
    SDL_Rect duplicatedBackgroundRect = {0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT + 10};
	Text score;
	Text realTimeGameInfo;
	uint32_t startFrameTime = 0;
	uint32_t endFrameTime = 0;

	bool quit = false;
    bool loopBreak = false;
    bool gamePaused = false;
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
//    Load sprite sheet (array of images)
	cannon->loadSpriteSheet("media/c1.bmp");
	cannon->loadSpriteSheet("media/c2.bmp");
	cannon->loadSpriteSheet("media/c3.bmp");
	cannon->loadSpriteSheet("media/c4.bmp");
	cannon->loadSpriteSheet("media/c5.bmp");
	cannon->loadSpriteSheet("media/c6.bmp");
	cannon->loadSpriteSheet("media/c7.bmp");
	cannon->loadSpriteSheet("media/c8.bmp");
	cannon->loadSpriteSheet("media/c9.bmp");
	cannon->loadBaseImage();
//    Start stopwatch to know how much time has elapsed when moving
	cannon->getStopwatch()->start();

//    Create enemies - flying squares
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

//    Cannon center position
	FlyingObject point(px, py, 25, 25, 0, "media/green1-square.bmp", gRenderer, "media/red-square.bmp");

	score.font = TTF_OpenFont("media/emulogic.ttf", 20);
	realTimeGameInfo.font = TTF_OpenFont("media/emulogic.ttf", 20);

	realTimeGameInfo.rect.x = 220;
	realTimeGameInfo.rect.y = 10;
	realTimeGameInfo.rect.w = 600;
	realTimeGameInfo.rect.h = 25;

	if (score.font == NULL || realTimeGameInfo.font == NULL)
	{
		std::cout << "Error: " << TTF_GetError() << endl;
		return -1;
	}

	backgroundSurf = IMG_Load("media/desert.jpg");
	backgroundText = SDL_CreateTextureFromSurface(gRenderer, backgroundSurf);

	//    hope the error is saved
	musicPlayer->createPlayList();
//	musicPlayer->playCurrPlaylist();

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) quit = true;
			else if (e.type == SDL_KEYDOWN)
			{
                if (gamePaused)
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_p:
                            gamePaused = !gamePaused;
                            break;
                    }
                }
				else
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
                        case SDLK_p:
                            gamePaused = !gamePaused;
                            break;
                        default:
                            break;
                    }
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

        if (gamePaused)
        {
            startFrameTime = SDL_GetTicks();
            totalFrames++;
            SDL_RenderClear(gRenderer);

//        Scrolling background implementation
            backgroundRect.y += 3;
            duplicatedBackgroundRect.y += 3;
            if(backgroundRect.y > SCREEN_HEIGHT) backgroundRect.y = 0;
            if(duplicatedBackgroundRect.y > 0) duplicatedBackgroundRect.y = -SCREEN_HEIGHT;

//        Background image should be the first one to be rendered, otherwise overwrites renders
            SDL_RenderCopy(gRenderer, backgroundText, NULL, &backgroundRect);
            SDL_RenderCopy(gRenderer, backgroundText, NULL, &duplicatedBackgroundRect);

//        Calculate last frame duration
            currentFrameTime = SDL_GetTicks();
            deltaTime = (float) (currentFrameTime - lastFrameTime) / 1000;
            lastFrameTime = SDL_GetTicks();

            for (size_t i = 0; i < ovnis.size(); i++)
            {
                int randomShot;

                if(ovnis[i].position.x > SCREEN_WIDTH)
                {
                    ovnis[i].position.x = 0;
                    ovnis[i].position.y = Util::GenerateRandom(0, SCREEN_HEIGHT / 4) + Util::GenerateRandom(0, SCREEN_HEIGHT / 4);
                    ovnis[i].stopFalling();
                }
                else if(ovnis[i].isIsFalling())
                {
//				FIXME check why 99 works and 9.8 doesn't
                    ovnis[i].updateSpeedX(0, deltaTime);
                    ovnis[i].updateSpeedY(99, deltaTime);
                    ovnis[i].moveX(deltaTime);
                    ovnis[i].moveY(deltaTime);
                }
                else
                {
                    randomShot = Util::GenerateRandom(0, 100);
                    //cout << randomShot << endl;
                    ovnis[i].moveX(deltaTime);
                    if(randomShot > 99)
                    { // 1% de chance de atirar
                        ovnis[i].setDeltaTime(deltaTime);
                        ovnis[i].fire(cannon->position);
                    }
                }
                ovnis[i].setDT(deltaTime);
                ovnis[i].draw();
            }
//        Animate using spritesheet
            cannon->show(0.0f);

            if(rotation == ROTATE_LEFT) cannon->rotateLeft(gRenderer);
            else if(rotation == ROTATE_RIGHT) cannon->rotateRight(gRenderer);
            else cannon->draw(gRenderer);

            // \todo Implement a way of preventing same event processing in different frames
            /** Draw bullets
        * Detect collision between cannon bullets and ovnis (and play sound effect if any)
        * Increase score
        */
            loopBreak = false;
		cannon->
                drawBullets(gRenderer);
		for (
                size_t i = 0; i < cannon->bullets.size() && !loopBreak; i++)
		{
            for (size_t j = 0; j < ovnis.size() && !loopBreak; j++) {
				if (Collision::CircleCollision(ovnis[j].position, cannon->bullets[i].position)) {
                    ovnis[j].fall();
                    kills++;
                    cannon->bullets.at(i).position.y = 0;
                    musicPlayer->playSoundEffect("media/effect.wav");
                    loopBreak = true;
				}
//				if (Collision::AABBCollision(&ovnis[i].position, &cannon->bullets[i].position))
//				{
//					kills++;
//					musicPlayer->playSoundEffect("media/effect.wav");
//                    loopBreak = true;
//				}
            }
		}

//        Collision between ovnis shoots and the cannon
            loopBreak = false;
            for (size_t k = 0; k < ovnis.size() && !loopBreak; ++k)
        {
            for (size_t m = 0; m < ovnis[k].bullets.size() && !loopBreak; ++m)
            {
//                CircleCollision is better than AABB here
                if (Collision::CircleCollision(cannon->position, ovnis[k].bullets[m].position))
                {
                    cannon->setLifes(cannon->getLifes() - 1);
                    musicPlayer->playSoundEffect("media/effect.wav");
                    ovnis[k].bullets.erase(ovnis[k].bullets.begin() + m);
                    loopBreak = true;
                }
            }
        }
//        if (cannon->getLifeState() == Cannon::LIFE_STATE::DEAD)
//        {
//            cout << "dead..." << endl;
//        }
            if(kills >= 1000) kills = 0;
            std::stringstream temp;
            temp << kills;

            //        FIXME Poor performance
            score.displayText = "Kills: " + temp.str();

            std::stringstream deltaString;
            deltaString << deltaTime;

            std::stringstream waitTime;
            waitTime << fpsMill;

            std::stringstream fspString;
            fspString << fps;

            realTimeGameInfo.displayText = "DELTA: " + deltaString.str() + "  WAIT: " + waitTime.str() +
                    "\n  FPS: " + fspString.str();

            score.surface = TTF_RenderText_Solid(score.font, score.displayText.c_str(), score.color);
            realTimeGameInfo.surface = TTF_RenderText_Solid(realTimeGameInfo.font,
                    realTimeGameInfo.displayText.c_str(), realTimeGameInfo.color);

            if(score.surface == NULL || realTimeGameInfo.surface == NULL)
            {
                std::cout << "Error:" << TTF_GetError() << endl;
                return -1;
            }

            score.texture = SDL_CreateTextureFromSurface(gRenderer, score.surface);
            realTimeGameInfo.texture = SDL_CreateTextureFromSurface(gRenderer, realTimeGameInfo.surface);

            if(rotation == ROTATE_LEFT) cannon->rotateLeft(gRenderer);
            else if(rotation == ROTATE_RIGHT) cannon->rotateRight(gRenderer);
            else cannon->draw(gRenderer);

            SDL_RenderCopy(gRenderer, score.texture, NULL, &score.rect);
            SDL_RenderCopy(gRenderer, realTimeGameInfo.texture, NULL, &realTimeGameInfo.rect);

//        Draw cannon center position
            point.position.h = 3;
            point.position.w = 3;
            point.position.x = cannon->position.x + cannon->position.w / 2;
            point.position.y = cannon->position.y + cannon->position.h / 2;
            point.draw();

            SDL_RenderPresent(gRenderer);
            //std::cout << fpsMill << " dt " << deltaTime << endl;

            SDL_Delay(fpsMill - deltaTime);
        }
	}
	close();
	return 0;
}
