//
//  Audio.cpp
//  Link
//
//  Created by Fuatnow on 15-4-2.
//
//

#include "Audio.h"
static Audio* instance = NULL;

Audio* Audio::getInstance()
{
    if(instance == NULL)
    {
        instance = new Audio;
        instance->musicPlay = userSave->getBoolForKey("music",true);
        instance->soundPlay = userSave->getBoolForKey("sound",true);
        instance->hadMusicPlay = false;
    }
    return instance;
}

bool Audio::getMusicCanPlay()
{
    return musicPlay;
}

bool Audio::getSoundCanPlay()
{
    return soundPlay;
}

void Audio::setMusicCanPlay(bool canPlay)
{
    musicPlay = canPlay;
    if (musicPlay)
    {
        if (hadMusicPlay)
        {
            audioEngine->resumeBackgroundMusic();
        }
        else
        {
            hadMusicPlay = true;
            audioEngine->playBackgroundMusic(DefaultMuisc,true);
        }
    }
    else
    {
        audioEngine->pauseBackgroundMusic();
    }
    userSave->setBoolForKey("music",musicPlay);
}

void Audio::setSoundCanPlay(bool canPlay)
{
    soundPlay = canPlay;
    userSave->setBoolForKey("sound",soundPlay);
}

void Audio::playMusic(std::string fileName, bool isLoop)
{
    if (musicPlay)
    {
        hadMusicPlay = true;
        audioEngine->pauseBackgroundMusic();
        audioEngine->playBackgroundMusic(fileName.c_str(),isLoop);
    }
}

void Audio::playEffect(std::string fileName)
{
    if(soundPlay)
    {
        audioEngine->playEffect(fileName.c_str());
    }
}

void Audio::purgeInstance()
{
    delete instance;
    instance = NULL;
}

void Audio::stopAllEffects()
{
    audioEngine->stopAllEffects();
}