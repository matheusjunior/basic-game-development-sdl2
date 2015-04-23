//
// Created by Matheus Pereira Junior on 3/27/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __MusicPlayer_H_
#define __MusicPlayer_H_


#ifdef _WIN32

#include <SDL_mixer.h>

#else

#include <SDL2_mixer/SDL_mixer.h>

#endif


#include <vector>

class MusicPlayer
{
private:
    int freq;
    int quantChannels;
    int chunksize;
    int fadeInTime;
    short volume;
    short musicIndex;
    Mix_Music* playingMusic;
    std::vector<Mix_Music*> playlist;
    Mix_Chunk* soundEffect; /// SDL type for audio chunk

public:
    MusicPlayer();


    Mix_Chunk *getSoundEffect() const
    {
        return soundEffect;
    }

    void setSoundEffect(Mix_Chunk *soundEffect)
    {
        MusicPlayer::soundEffect = soundEffect;
    }

    short getVolume() const
    {
        return volume;
    }

    void setVolume(short volume)
    {
        MusicPlayer::volume = volume;
    }

    int getFadeInTime() const
    {
        return fadeInTime;
    }

    void setFadeInTime(int fadeInTime)
    {
        MusicPlayer::fadeInTime = fadeInTime;
    }
    Mix_Music *getPlayingMusic() const
    {
        return playingMusic;
    }

    void setPlayingMusic(Mix_Music *playingMusic)
    {
        MusicPlayer::playingMusic = playingMusic;
    }

    int getFreq() const
    {
        return freq;
    }

    void setFreq(int freq)
    {
        MusicPlayer::freq = freq;
    }

    int getQuantChannels() const
    {
        return quantChannels;
    }

    void setQuantChannels(int quantChannels)
    {
        MusicPlayer::quantChannels = quantChannels;
    }

    int getChunksize() const
    {
        return chunksize;
    }

    void setChunksize(int chunksize)
    {
        MusicPlayer::chunksize = chunksize;
    }

    std::vector<Mix_Music *> const &getPlaylist() const
    {
        return playlist;
    }

    void setPlaylist(std::vector<Mix_Music *> const &playlist)
    {
        MusicPlayer::playlist = playlist;
    }

    int playMusic(const char* musicPath);
    int loadMusic(const char* musicPath);
    int playSoundEffect(const char* soundEffectPath);
    int loadSoundEffect(const char* soundEffectPath);
//    TODO Playlist with name
    int createPlayList();
    void playCurrPlaylist();
//    Just valid for playlists
    void nextMusic();
    void previousMusic();
    int playMusicFadeInEffect(const char*path);
    void increaseVolume();
    void decreaseVolume();
};


#endif //__MusicPlayer_H_
