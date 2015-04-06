//
//  SuccessDialog.cpp
//  Link
//
//  Created by Fuatnow on 15-3-30.
//
//

#include "SuccessDialog.h"
#include "GameLayer.h"
#include "LevelLayer.h"
bool SuccessDialog::init()
{
    Node::init();
    initData();
    return true;
}


void SuccessDialog::initData()
{
    
    Node* root = CSLoader::createNode("SuccessDialog.csb");
    auto size = Director::getInstance()->getWinSize();
    root->setPosition(size/2);
    this->addChild(root);
    
    int level = GameData::getInstance()->getLevel();
    auto levelLabel = (Text*)seekNodeByName(root, "levelLabel");
    std::stringstream ss;
    ss << level ;
    levelLabel->setString(ss.str());
    
    auto playerBestLabel = (TextBMFont*)seekNodeByName(root, "playerBestLabel");
    playerBestLabel->setString("0");
    
    auto selfBestLabel = (TextBMFont*)seekNodeByName(root, "selfBestLabel");
    selfBestLabel->setString("1");
    
    auto gainCoinLabel = (TextBMFont*)seekNodeByName(root, "gainCoinLabel");
    gainCoinLabel->setString("2");
    
    float runTime = 0.1f;
    
    auto star1 = (Node*)seekNodeByName(root, "star1");
    star1->setScale(30);
    star1->setOpacity(10);
    auto pos = star1->getPosition();
    star1->setPosition(pos+Vec2(0,-200));
    auto moveTo = MoveTo::create(runTime, pos);
    auto scaleTo = ScaleTo::create(runTime, 1.0f);
    auto fadeIn = FadeIn::create(runTime);
    auto spawn = Spawn::create(scaleTo,moveTo,fadeIn, NULL);
    auto callBack = CallFuncN::create(CC_CALLBACK_1(SuccessDialog::star_callBack,this));
    auto starAct = Sequence::create(spawn, callBack,NULL);
    star1->runAction(starAct);
    
    auto star2 = (Node*)seekNodeByName(root, "star2");
    star2->setScale(30);
    star2->setOpacity(30);
    pos = star2->getPosition();
    star2->setPosition(pos+Vec2(0,-200));
    star2->runAction(Sequence::create(DelayTime::create(runTime),starAct,NULL));
    
    auto star3 = (Node*)seekNodeByName(root, "star3");
    star3->setScale(10);
    star3->setOpacity(10);
    pos = star3->getPosition();
    star3->setPosition(pos+Vec2(0,-200));
    star3->runAction(Sequence::create(DelayTime::create(runTime*2),starAct,NULL));
    
    
    
    auto replayBtn = (Button*)seekNodeByName(root, "replayBtn");
    replayBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
    
    
    auto nextBtn = (Button*)seekNodeByName(root, "nextBtn");
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
    
    auto closeBtn = (Button*)seekNodeByName(root, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
}


void SuccessDialog::star_callBack(cocos2d::Ref *pSender)
{
    //播放音xiao
    CCLOG("star_callBack");
}

void SuccessDialog::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    
    if(type == Widget::TouchEventType::ENDED)
    {
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        switch (tag)
        {
            case 1:
                CCLOG("replay_callBack");
                {
                    auto sc = GameLayer::scene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                }
                break;
            case 2:
                CCLOG("next_callBack");
                {
                    int curLevel = GameData::getInstance()->getLevel();
                    int maxLevel = GameData::getInstance()->getMaxLevel();
                    if(curLevel ==  maxLevel)
                    {
                        GameData::getInstance()->setLevel(curLevel+1);
                        GameData::getInstance()->setMaxLevel(curLevel+1);
                        auto sc = Scene::create();
                        auto layer = LevelLayer::create();
                        sc->addChild(layer);
                        int prePlayLevel = curLevel;
                        layer->toTargetLevel(prePlayLevel, prePlayLevel+1);
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                    }
                    else
                    {
                        GameData::getInstance()->setLevel(curLevel+1);
                        auto sc = GameLayer::scene();
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                    }
                }
                break;
            case 3:
                CCLOG("close_callBack");
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
