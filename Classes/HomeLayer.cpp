//
//  HomeLayer.cpp
//  Link
//
//  Created by Fuatnow on 15-3-27.
//
//

#include "HomeLayer.h"
#include "LevelLayer.h"
#include "Audio.h"
Scene* HomeLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  HomeLayer::create();
    sc->addChild(layer);
    return sc;
}

bool HomeLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void HomeLayer::initData()
{
    
}

void HomeLayer::initView()
{
    auto backgroundLayer = BackgroundLayer::create();
    addChild(backgroundLayer,-1);
    
    auto rootNode =  CSLoader::createNode("HomeLayer.csb");
    this->addChild(rootNode);
    
    
    
    startItem = (Button*)seekNodeByName(rootNode, "playButton");
    startItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::start_callBack, this));
    
    
    bool musicCanPlay = Audio::getInstance()->getMusicCanPlay();
    bool soundCanPlay = Audio::getInstance()->getSoundCanPlay();
    
    musicItem = (Button*)seekNodeByName(rootNode, "musicItem_on");
    musicItem->setVisible(musicCanPlay);
    musicItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::music_callBack, this));
    musicItem_off = (Button*)seekNodeByName(rootNode, "musicItem_off");
    musicItem_off->addTouchEventListener(CC_CALLBACK_2(HomeLayer::music_callBack, this));
    musicItem_off->setVisible(!musicCanPlay);
   
    
    
    soundItem = (Button*)seekNodeByName(rootNode, "soundItem_on");
    soundItem->setVisible(soundCanPlay);
    soundItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::sound_callBack, this));
    soundItem_off = (Button*)seekNodeByName(rootNode, "soundItem_off");
    soundItem_off->addTouchEventListener(CC_CALLBACK_2(HomeLayer::sound_callBack, this));
    soundItem_off->setVisible(!soundCanPlay);
    
    Audio::getInstance()->playMusic(DefaultMuisc, true);
}


void HomeLayer::start_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        CCLOG("srat_callBack");
        Audio::getInstance()->playEffect(sound_click2);
        auto move = MoveBy::create(0.5, Vec2(-0.3f*winWidth,0));
        musicItem->runAction(move->clone());
        musicItem_off->runAction(move->clone());
        soundItem->runAction(move->reverse()->clone());
        soundItem_off->runAction(move->reverse()->clone());
        auto play = (Node*)pSender;
        auto moveUp = MoveBy::create(0.5f, Vec2(0,0.7f*winHeight));
        //不起作用
        play->stopAllActions();
        //play->runAction(moveUp);
        
        auto sc = LevelLayer::scene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, sc));
    }
}

void HomeLayer::music_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    {
        CCLOG("music_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        musicItem->setVisible(!tag);
        musicItem_off->setVisible(tag);
        Audio::getInstance()->playEffect(sound_click);
        Audio::getInstance()->setMusicCanPlay(!tag);
    }
}

void HomeLayer::sound_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    {
        CCLOG("sound_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        soundItem->setVisible(!tag);
        soundItem_off->setVisible(tag);
        Audio::getInstance()->setSoundCanPlay(!tag);
        Audio::getInstance()->playEffect(sound_click);
    }
}