#ifdef _WIN32
#include <SDL.h>
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
#include "MusicPlayer.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
Mix_Music* music = NULL;
MusicPlayer* musicPlayer = NULL;

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
        cout << "Error while initializing SDL_Ttf: %s" << TTF_GetError() << endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

    return true;
}

/* Load a bmp image into a SDL_Surface and create a SDL_Texture from it
* @param path image path
* @return SDL_Texture containing the image in path if succeeds, NULL otherwise
* */
SDL_Texture *getTexture(string path)
{
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    cout << SDL_GetError();
    SDL_Texture *text = SDL_CreateTextureFromSurface(gRenderer, surface);

    if (text == NULL) cout << SDL_GetError() << endl;
    if (Mix_OpenAudio(musicPlayer->getFreq(), MIX_DEFAULT_FORMAT,
            musicPlayer->getQuantChannels(), musicPlayer->getChunksize()) == NULL) cout << Mix_GetError() << endl;

    return text;
}

/* Close SDL resources in use
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

/* Game main function. All event handling is first processed here
* and delegated to classes if needed.
* @return 0 if succeeds, -1 otherwise
* */
int main(int argc, char *args[])
{
    Text text;
    bool quit = false;
    uint32_t startFrameTime;
    uint32_t endFrameTime;
    SDL_Event e;
    const Uint8 *currKeyStates;
    int kills = 0;
    int musicVolume = 70;
    musicPlayer = new MusicPlayer();

    if(!init())
    {
        cout << "Falhou init\n";
        return -1;
    }

    Cannon *cannon = new Cannon(SCREEN_WIDTH / 6, SCREEN_WIDTH / 2, 100, 80, 400);
    GameObject *spider = new GameObject(SCREEN_WIDTH - 200, SCREEN_WIDTH / 6, 102, 106, 400);
    GameObject *fly = new GameObject(SCREEN_WIDTH / 6, SCREEN_WIDTH / 6, 96, 116, 400);

    cannon->texture = getTexture("media/cannon.bmp");
    spider->texture = getTexture("media/spider.bmp");
    fly->texture = getTexture("media/fly.bmp");

    text.font = TTF_OpenFont("sample.ttf", 20);
    if(text.font == NULL)
    {
        cout << "Error: " << TTF_GetError() << endl;
        return -1;
    }

//    hope the error is saved
//    if (musicPlayer->playMusic("media/02-las-vegas.mp3") == -1) cout << Mix_GetError() << endl;
//    musicPlayer->playMusicFadeInEffect("media/02-las-vegas.mp3");
    musicPlayer->createPlayList();
    musicPlayer->playCurrPlaylist();

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = true;
        }
        currKeyStates = SDL_GetKeyboardState(NULL);

        if (currKeyStates[SDL_SCANCODE_LEFT]) cannon->moveX(-10);
        if (currKeyStates[SDL_SCANCODE_RIGHT]) cannon->moveX(10);
        if (currKeyStates[SDL_SCANCODE_UP]) cannon->moveY(-10);
        if (currKeyStates[SDL_SCANCODE_DOWN]) cannon->moveY(10);
        if (currKeyStates[SDL_SCANCODE_SPACE]) cannon->fire();

        if (currKeyStates[SDL_SCANCODE_U]) musicPlayer->decreaseVolume();
        if (currKeyStates[SDL_SCANCODE_I]) musicPlayer->increaseVolume();
        if (currKeyStates[SDL_SCANCODE_B]) musicPlayer->previousMusic();
        if (currKeyStates[SDL_SCANCODE_N]) musicPlayer->nextMusic();


        startFrameTime = SDL_GetTicks();
        SDL_RenderClear(gRenderer);

        if(fly->position.x > SCREEN_WIDTH) fly->position.x = 0;
        else fly->moveX(15);
        fly->draw(gRenderer);
        cannon->draw(gRenderer);

        for (int i = 0; i < cannon->bullets.size(); ++i)
        {
            if (Collision::AABBCollision(&fly->position, &cannon->bullets[i].position))
            {
                kills++;
                musicPlayer->playSoundEffect("media/effect.wav");
                break; // cannot kill twice in a row
            }
        }
        if(kills >= 1000) kills = 0;
        std::stringstream temp;
        temp << kills;

//        FIXME Poor performance
        text.displayText = "Kills: " + temp.str();
        text.surface = TTF_RenderText_Solid(text.font, text.displayText.c_str(), text.color);
        if(text.surface == NULL)
        {
            cout << "Error:" << TTF_GetError() << endl;
            return -1;
        }
        text.texture = SDL_CreateTextureFromSurface(gRenderer, text.surface);
        SDL_RenderCopy(gRenderer, text.texture, NULL, &text.rect);

        endFrameTime = SDL_GetTicks();

        SDL_RenderPresent(gRenderer);
        SDL_Delay(30 - (endFrameTime - startFrameTime));
    }
    close();
    return 0;
}
