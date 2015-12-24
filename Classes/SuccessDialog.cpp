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
#include "RankLayer.h"
#include "Toast.h"
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
    
    
    int starNum = GameData::getInstance()->getCurLevelStar();
    int curScore = GameData::getInstance()->getCurLevelScore();
    ss.clear();
    ss.str("");
    ss << curScore;
    auto curScoreLabel = (TextBMFont*)seekNodeByName(root, "playerBestLabel");
    curScoreLabel->setString(ss.str());
    
    int selfBestNum =  GameData::getInstance()->getBestScoreNum();
    ss.clear();
    ss.str("");
    ss << selfBestNum;
    auto selfBestLabel = (TextBMFont*)seekNodeByName(root, "selfBestLabel");
    selfBestLabel->setString(ss.str());
    
    gainCoinNum = starNum * starNum * 10;
    GameData::getInstance()->addCoin(gainCoinNum);
    auto gainCoinLabel = (TextBMFont*)seekNodeByName(root, "gainCoinLabel");
    ss.clear();
    ss.str("");
    ss << gainCoinNum;
    gainCoinLabel->setString(ss.str());
    
    float runTime = 0.1f;
    if(starNum > 0)
    {
        auto star1 = (Node*)seekNodeByName(root, "star1");
        star1->setVisible(true);
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
        
        if(starNum > 1)
        {
            auto star2 = (Node*)seekNodeByName(root, "star2");
            star2->setVisible(true);
            star2->setScale(30);
            star2->setOpacity(30);
            pos = star2->getPosition();
            star2->setPosition(pos+Vec2(0,-200));
            star2->runAction(Sequence::create(DelayTime::create(runTime),starAct,NULL));
        }
        
        if(starNum > 2)
        {
            auto star3 = (Node*)seekNodeByName(root, "star3");
            star3->setVisible(true);
            star3->setScale(10);
            star3->setOpacity(10);
            pos = star3->getPosition();
            star3->setPosition(pos+Vec2(0,-200));
            star3->runAction(Sequence::create(DelayTime::create(runTime*2),starAct,NULL));
        }
    }
    
    
    
    auto replayBtn = (Button*)seekNodeByName(root, "replayBtn");
    replayBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
    
    
    auto nextBtn = (Button*)seekNodeByName(root, "nextBtn");
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
    
    auto rankBtn = (Button*)seekNodeByName(root, "rankBtn");
    rankBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
    
    auto closeBtn = (Button*)seekNodeByName(root, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(SuccessDialog::btn_callBack, this));
}


void SuccessDialog::star_callBack(cocos2d::Ref *pSender)
{
    //播放音xiao
    log("star_callBack");
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
                log("replay_callBack");
                {
                    auto sc = GameLayer::scene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                }
                break;
            case 2:
                log("next_callBack");
                {
                    int curLevel = GameData::getInstance()->getLevel();
                    int maxLevel = GameData::getInstance()->getMaxLevel();
                    
                    if(maxLevel == 150)
                    {
                        Toast::toast("你已经通关");
                        return;
                    }
                    
                    
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
                log("rank_callBack");
                {
                    GameData::getInstance()->setQuaryTotalScore(false);
                    auto rankLayer = RankLayer::create();
                    int curLevel = GameData::getInstance()->getLevel();
                    rankLayer->loadLeaderboard(curLevel);
                    addChild(rankLayer,curLevel);
                }
                break;
            case 4:
                log("close_callBack");
                {
//                    int curLevel = GameData::getInstance()->getLevel();
//                    int maxLevel = GameData::getInstance()->getMaxLevel();
//                    
//                    if(maxLevel == 150)
//                    {
//                        Toast::toast("你已经通关");
//                        return;
//                    }
//                    
//                    
//                    if(curLevel ==  maxLevel)
//                    {
//                        GameData::getInstance()->setLevel(curLevel+1);
//                        GameData::getInstance()->setMaxLevel(curLevel+1);
//                    }
                    auto sc = LevelLayer::scene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                }
                break;
            default:
                break;
        }
    }
    
}

