//
//  LoseDialog.cpp
//  Link
//
//  Created by Fuatnow on 15-4-1.
//
//

#include "LoseDialog.h"
#include "GameLayer.h"
#include "LevelLayer.h"
bool LoseDialog::init()
{
    Node::init();
    initData();
    return true;
}


void LoseDialog::initData()
{
    
    Node* root = CSLoader::createNode("PauseDialog.csb");
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
    ss << level;
    levelLabel->setString(ss.str());
    
    //1
    auto replayBtn = (Button*)seekNodeByName(root, "replayBtn");
    replayBtn->addTouchEventListener(CC_CALLBACK_2(LoseDialog::btn_callBack, this));

    //tag 6
    auto homeBtn = (Button*)seekNodeByName(root, "homeBtn");
    homeBtn->addTouchEventListener(CC_CALLBACK_2(LoseDialog::btn_callBack, this));
    
    
    //8
    auto closeBtn = (Button*)seekNodeByName(root, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(LoseDialog::btn_callBack, this));
}

void LoseDialog::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    int tag = btn->getTag();
    if(type == Widget::TouchEventType::ENDED)
    {
        switch (tag)
        {
            case 1:
                CCLOG("replay_callBack");
            {
                auto sc = GameLayer::scene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
            }
                break;
            case 6:
                CCLOG("homeBtn");
            {
                auto sc = LevelLayer::scene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
            }
                break;
            case 8:
                CCLOG("closeBtn");
            {
                auto sc = LevelLayer::scene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
            }
                break;
            default:
                break;
        }
    }
    
}