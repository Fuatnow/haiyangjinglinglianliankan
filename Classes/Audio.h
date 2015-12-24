//
//  Audio.h
//  Link
//
//  Created by Fuatnow on 15-4-2.
//
//

#ifndef __Link__Audio__
#define __Link__Audio__

#include "BasicLayer.h"
#include "Define.h"
#include "Resounce.h"
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

class Audio
{
public:
    Audio(){};
    static Audio* getInstance();
    bool getMusicCanPlay();
    bool getSoundCanPlay();
    void purgeInstance();
    void setMusicCanPlay(bool canPlay);
    void setSoundCanPlay(bool canPlay);
    void playMusic(std::string fileName, bool isLoop);
    void playEffect(std::string fileName);
    void stopAllEffects();
private:
    bool musicPlay;
    bool soundPlay;
    bool hadMusicPlay;
};

#endif /* defined(__Link__Audio__) */
