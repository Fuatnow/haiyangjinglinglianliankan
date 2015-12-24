//
//  PauseDialog.cpp
//  Link
//
//  Created by Fuatnow on 15-4-1.
//
//

#include "PauseDialog.h"
#include "GameLayer.h"
#include "LevelLayer.h"
#include "Audio.h"
bool PauseDialog::init()
{
    Node::init();
    initData();
    return true;
}


void PauseDialog::initData()
{
    
    Node* root = CSLoader::createNode("LoseDialog.csb");
    auto size = Director::getInstance()->getWinSize();
    root->setPosition(size/2);
    this->addChild(root);
    
    auto bg = (Sprite*)seekNodeByName(root, "bg");
    bg->setScale(0.2f);
    bg->setOpacity(20);
    auto scaleTo1 = ScaleTo::create(0.2f, 1.2f);
    auto scaleTo2 = ScaleTo::create(0.1f, 0.9f);
    auto scaleTo3 = ScaleTo::create(0.05f, 1.0f);
    auto fadeIn = FadeIn::create(0.2f);
    auto seq = Sequence::create(scaleTo1,scaleTo2,scaleTo3, NULL);
    bg->runAction(seq);
    bg->runAction(fadeIn);
    
    int level = GameData::getInstance()->getLevel();
    auto levelLabel = (Text*)seekNodeByName(root, "levelLabel");
    std::stringstream ss;
    ss << level ;
    levelLabel->setString(ss.str());
  

    //tag 1
    auto replayBtn = (Button*)seekNodeByName(root, "replayBtn");
    replayBtn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 7
    auto resumeBtn = (Button*)seekNodeByName(root, "playButton");
    resumeBtn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 8
    auto closeBtn = (Button*)seekNodeByName(root, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 4
    soundBtnOn = (Button*)seekNodeByName(root, "soundBtnOn");
    soundBtnOn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 5
    soundBtnOff = (Button*)seekNodeByName(root, "soundBtnOff");
    soundBtnOff->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 2
    musicBtnOn = (Button*)seekNodeByName(root, "musicBtnOn");
    musicBtnOn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 3
    musicBtnOff = (Button*)seekNodeByName(root, "musicBtnOff");
    musicBtnOff->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
    //tag 6
    auto homeBtn = (Button*)seekNodeByName(root, "homeBtn");
    homeBtn->addTouchEventListener(CC_CALLBACK_2(PauseDialog::btn_callBack, this));
    
}

void PauseDialog::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    int tag = btn->getTag();
    if(type == Widget::TouchEventType::BEGAN)
    {
        switch (tag)
        {
            case 2:
                log("musicBtnOn");
            {
                musicBtnOn->setVisible(false);
                musicBtnOff->setVisible(true);
                
                Audio::getInstance()->playEffect(sound_click);
                Audio::getInstance()->setMusicCanPlay(false);
            }
                break;
            case 3:
                log("musicBtnOff");
            {
                musicBtnOff->setVisible(false);
                musicBtnOn->setVisible(true);
                
                Audio::getInstance()->playEffect(sound_click);
                Audio::getInstance()->setMusicCanPlay(true);
            }
                break;
            case 4:
                log("soundBtnOn");
            {
                soundBtnOn->setVisible(false);
                soundBtnOff->setVisible(true);
                
                Audio::getInstance()->setSoundCanPlay(false);
                Audio::getInstance()->playEffect(sound_click);
            }
                break;
            case 5:
                log("soundBtnOff");
            {
                soundBtnOff->setVisible(false);
                soundBtnOn->setVisible(true);
                
                Audio::getInstance()->setSoundCanPlay(true);
                Audio::getInstance()->playEffect(sound_click);
            }
                break;
        }
    }
    
    if(type == Widget::TouchEventType::ENDED)
    {
        switch (tag)
        {
            case 1:
                log("replay_callBack");
            {
                auto sc = GameLayer::scene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
            }
                break;
            case 6:
                log("homeBtn");
            {
                auto sc = LevelLayer::scene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
            }
                break;
            case 7:
                log("playButton");
            {
                auto game = (GameLayer*)getParent();
                game->resumeGame();
                removeFromParentAndCleanup(true);
            }
                break;
            case 8:
                log("closeBtn");
            {
                auto game = (GameLayer*)getParent();
                game->resumeGame();
                removeFromParentAndCleanup(true);
            }
                break;
            default:
                break;
        }
    }
    
}