#ifdef _WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>

#else

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
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

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

using namespace std;

/* Initialize SDL components
* @return true if: video component initialized properly, window succesfully created,
* and SDL_Tff is properly initialized
* false otherwise
* */
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

    gWindow = SDL_CreateWindow("Cannon Game !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL) return false;
    if (TTF_Init() == -1)
    {
        std::cout << "Error while initializing SDL_Ttf: %s" << TTF_GetError() << endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    return true;
}

/* Close SDL resources in use
* */
void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    SDL_Quit();
}

/* Game main function. All event handling is first processed here
* and delegated to classes if needed.
* @return 0 if succeeds, -1 otherwise
* */
int main(int argc, char *args[])
{
	srand(time(NULL));

    Text text;
	Text textFPS;
    bool quit = false;
    uint32_t startFrameTime = 0;
    uint32_t endFrameTime = 0;
    float startMoveTime = 0; // Define the move time step in sec to move the obj
    float endMoveTime = 0;
    float dMoveTime = 0;
    SDL_Event e;
    const Uint8 *currKeyStates;
    int kills = 0;

	float deltaTime;
	int lastFrameTime = 0, currentFrameTime = 0;
	int fps = 38;
	int fpsMill = 1000 / fps;
	int totalFrames = fps;


    if(!init())
    {
        std::cout << "Falhou init\n";
        return -1;
    }

    Cannon *cannon = new Cannon(SCREEN_WIDTH / 6, SCREEN_WIDTH / 2, 100, 80, 400);
    GameObject *spider = new GameObject(SCREEN_WIDTH - 200, SCREEN_WIDTH / 6, 102, 106, 400);
    GameObject *fly = new GameObject(SCREEN_WIDTH / 6, SCREEN_WIDTH / 6, 96, 116, 400);

	int px = -25 +  Util::GenerateRandom(0, 60);
	int py = Util::GenerateRandom(0, 70);
	int speed = 250;
	FlyingObject fly1(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");


	px = -25 + Util::GenerateRandom(1, 61);
	py = Util::GenerateRandom(60, 140);
	speed = 300;
	FlyingObject fly2(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	px = -25 + Util::GenerateRandom(2, 62);
	py = Util::GenerateRandom(120, 210);
	speed = 320;
	FlyingObject fly3(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	px = -55 + Util::GenerateRandom(0, 60);
	py = Util::GenerateRandom(200, 280);
	speed = 250;
	FlyingObject fly4(px, py, 25, 25, speed, "media/purple-square.bmp", gRenderer, "media/red-square.bmp");

	ovnis.push_back(fly1);
	ovnis.push_back(fly2);
	ovnis.push_back(fly3);
	ovnis.push_back(fly4);

    Cannon *cannon = new Cannon(SCREEN_WIDTH / 2 - 25, SCREEN_WIDTH / 2 + 108, 40, 40, 400, "media/green1-square.bmp", gRenderer, "media/red-square.bmp");
	
    text.font = TTF_OpenFont("emulogic.ttf", 20);
	textFPS.font = TTF_OpenFont("emulogic.ttf", 20);

	textFPS.rect.x = 220;
	textFPS.rect.y = 10;
	textFPS.rect.w = 600;
	textFPS.rect.h = 25;
	

    if(text.font == NULL || textFPS.font == NULL)
    {
        std::cout << "Error: " << TTF_GetError() << endl;
        return -1;
    }

	int currentSpeed = fps;
    
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE: // TODO Check break usage
                        quit = true;
                        break;
                    case SDLK_SPACE:
                        cannon->fire();
                        break;
                    case SDLK_RIGHT:
                        cannon->position.x += 7;
                        if(cannon->position.x + cannon->position.w > SCREEN_WIDTH) cannon->position.x = SCREEN_WIDTH - cannon->position.w;
                        break;
                    case SDLK_LEFT:
                        cannon->position.x -= 7;
                        if(cannon->position.x + cannon->position.w > SCREEN_WIDTH) cannon->position.x = SCREEN_WIDTH - cannon->position.w;
                        break;
                    default:
                        break;
                }
            }
        }
        startFrameTime = SDL_GetTicks();
        totalFrames++;
        SDL_RenderClear(gRenderer);

		currentFrameTime = SDL_GetTicks();

		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;

		lastFrameTime = SDL_GetTicks();


		for (size_t i = 0; i <ovnis.size(); i++) {
			bool singleShot = true;

			if (ovnis[i].position.x > SCREEN_WIDTH) {
				ovnis[i].position.x = 0;
				ovnis[i].position.y = Util::GenerateRandom(0, SCREEN_HEIGHT / 4) + Util::GenerateRandom(0, SCREEN_HEIGHT / 4);
				ovnis[i].stopFalling();
			}
			else {
				ovnis[i].moveX(deltaTime);
			}
			ovnis[i].draw();
		}

		
		//std::cout << deltaTime << endl;

        cannon->draw(gRenderer);

		
		for (size_t i = 0; i < cannon->bullets.size(); i++)
        {
			for (size_t j = 0; j < ovnis.size(); j++) {
				
				
				if (Collision::CircleCollision(ovnis[j].position, cannon->bullets[i].position)) {					
					
					ovnis[j].fall();
					kills++;
					cannon->bullets.at(i).position.y = 0;
					
					//isEmpty = cannon->bullets.empty();

                }
            }
        }
        if(kills >= 1000) kills = 0;
        std::stringstream temp;
        temp << kills;

//        FIXME Poor performance
        text.displayText = "Kills: " + temp.str();

		std::stringstream temp2;
		temp2 << deltaTime;

		std::stringstream temp3;
		temp3 << fpsMill;

		std::stringstream temp4;
		temp4 << fps;

		textFPS.displayText = "DELTA: " + temp2.str() + "  WAIT: " + temp3.str() + "\n  FPS: " + temp4.str();
		
        text.surface = TTF_RenderText_Solid(text.font, text.displayText.c_str(), text.color);
		textFPS.surface = TTF_RenderText_Solid(textFPS.font, textFPS.displayText.c_str(), textFPS.color);
        
		if(text.surface == NULL || textFPS.surface == NULL)
        {
            std::cout << "Error:" << TTF_GetError() << endl;
            return -1;
        }
        text.texture = SDL_CreateTextureFromSurface(gRenderer, text.surface);
		textFPS.texture = SDL_CreateTextureFromSurface(gRenderer, textFPS.surface);
        
		SDL_RenderCopy(gRenderer, text.texture, NULL, &text.rect);
		SDL_RenderCopy(gRenderer, textFPS.texture, NULL, &textFPS.rect);

        SDL_RenderPresent(gRenderer);
		
		//std::cout << fpsMill << " dt " << deltaTime << endl;

		SDL_Delay(fpsMill - deltaTime);
    }
    close();
    return 0;
}
