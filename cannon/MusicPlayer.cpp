//
// Created by Matheus Pereira Junior on 3/27/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include "MusicPlayer.h"
MusicPlayer::MusicPlayer()
{
    playingMusic = NULL;
    soundEffect = NULL;
    freq = 44100;
    quantChannels = 2;
    chunksize = 2048;
    fadeInTime = 2000;
    volume = 50;
    musicIndex = 0;
}

int MusicPlayer::playMusic(char const* path)
{
    if (playingMusic == NULL)
    {
        if (loadMusic(path) != 0)
        {
            return -1;
        };
    }

    if (Mix_PlayMusic(playingMusic, -1) == -1) return -1;
    return 0;
}

int MusicPlayer::playMusicFadeInEffect(const char*path)
{
    playingMusic = Mix_LoadMUS(path);
    if (playingMusic == NULL) return -1;
    return Mix_FadeInMusic(playingMusic, -1, 3000);
}

void MusicPlayer::increaseVolume()
{
    volume += 5;
    if (volume >= MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
}

void MusicPlayer::decreaseVolume()
{
    volume -= 5;
    if (volume <= 0) volume = 0;
    Mix_VolumeMusic(volume);
}

int MusicPlayer::loadMusic(const char *musicPath)
{
    playingMusic = Mix_LoadMUS("media/02-las-vegas.mp3");
    if (playingMusic == NULL) return -1;
    return 0;
}

// Need to use class method
int MusicPlayer::createPlayList()
{
    Mix_Music* music = Mix_LoadMUS("media/01-title.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/02-las-vegas.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/03-hiroshima.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/04-bordeaux.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/05-frankfurt.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/06-qualified.mp3");
    playlist.push_back(music);
    music = Mix_LoadMUS("media/07-ending-theme.mp3");
    playlist.push_back(music);
    return 0;
}

void MusicPlayer::nextMusic()
{
    if (Mix_PlayingMusic() == 0) Mix_PauseMusic();
    musicIndex++;
    if (musicIndex > playlist.size()) musicIndex = 0;
    playingMusic = playlist[musicIndex];
    Mix_PlayMusic(playingMusic, -1);
}

void MusicPlayer::previousMusic()
{
    if (Mix_PlayingMusic() == 0) Mix_PauseMusic();
    musicIndex--;
    if (musicIndex < 0) musicIndex = playlist.size() - 1;
    playingMusic = playlist[musicIndex];
    Mix_PlayMusic(playingMusic, -1);
}

void MusicPlayer::playCurrPlaylist()
{
    if (Mix_PlayingMusic() == 0) Mix_PauseMusic();
    playingMusic = playlist[0];
    Mix_PlayMusic(playingMusic, -1);
}

int MusicPlayer::playSoundEffect(const char *soundEffectPath)
{
    if (soundEffect == NULL)
    {
        if (loadSoundEffect(soundEffectPath) != 0)
        {
            return -1;
        };
    }

    Mix_PlayChannel(-1, soundEffect, 0);
    return 0;
}

int MusicPlayer::loadSoundEffect(const char *soundEffectPath)
{
    soundEffect = Mix_LoadWAV(soundEffectPath);
    if (soundEffect == NULL)
    {
        return -1;
    }
    return 0;
}
