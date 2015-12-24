//
//  AimDialog.cpp
//  Link
//
//  Created by Fuatnow on 15-3-30.
//
//

#include "AimDialog.h"
#include "GameLayer.h"
bool AimDialog::init()
{
    Node::init();
    initData();
    return true;
}


void AimDialog::initData()
{
    int level = GameData::getInstance()->getLevel();
    
    Node* root = CSLoader::createNode("LevelAimDialog.csb");
    auto size = Director::getInstance()->getWinSize();
    root->setPosition(size/2);
    this->addChild(root);
    
    
    auto levelLabel = (Text*)seekNodeByName(root, "levelLabel");
    std::stringstream ss;
    ss << level;
    levelLabel->setString(ss.str());
    
    auto playerBestLabel = (TextBMFont*)seekNodeByName(root, "playerBestLabel");
    playerBestLabel->setString("0");
    
    int selfBsetNum = GameData::getInstance()->getBestScoreNum();
    ss.clear();
    ss.str("");
    ss << selfBsetNum;
    auto selfBestLabel = (TextBMFont*)seekNodeByName(root, "selfBestLabel");
    selfBestLabel->setString(ss.str());
    
//    int bombNum = GameData::getInstance()->getBombNum();
//    int searchNum = GameData::getInstance()->getSearchNum();
//    int swapNum = GameData::getInstance()->getSwapNum();
//    int maxTakebombNum = GameData::getInstance()->getMaxTakeBombNum();
//    int maxTakeSearchNum = GameData::getInstance()->getMaxTakeSearchNum();
//    int maxTakeSwapNum = GameData::getInstance()->getMaxTakeSwapNum();
//    int takeBombNum = bombNum > maxTakebombNum ? maxTakebombNum : bombNum;
//    int takeSwapNum = swapNum > maxTakeSwapNum ? maxTakeSwapNum : swapNum;
//    int takeSearchNum = searchNum > maxTakeSearchNum ? maxTakeSearchNum : searchNum;
//    GameData::getInstance()->setTakeBombNum(takeBombNum);
//    GameData::getInstance()->setTakeSearchNum(takeSearchNum);
//    GameData::getInstance()->setTakeSwapNum(takeSwapNum);
//    GameData::getInstance()->setBombNum(bombNum - takeBombNum);
//    GameData::getInstance()->setSearchNum(searchNum - takeSearchNum);
//    GameData::getInstance()->setSwapNum(searchNum - takeSearchNum);
    
    int takeBombNum = GameData::getInstance()->getTakeBombNum();
    int takeSwapNum = GameData::getInstance()->getTakeSearchNum();
    int takeSearchNum = GameData::getInstance()->getTakeSearchNum();
    
    const char* bombNumStr = String::createWithFormat("x%d",takeBombNum)->getCString();
    const char* searchNumStr = String::createWithFormat("x%d",takeSearchNum)->getCString();
    const char* swapNumStr = String::createWithFormat("x%d",takeSwapNum)->getCString();
    auto swapNumLab = (TextBMFont*)seekNodeByName(root, "swapNumLab");
    swapNumLab->setString(swapNumStr);
    auto bombNumLab = (TextBMFont*)seekNodeByName(root, "bombNumLab");
    bombNumLab->setString(bombNumStr);
    auto searchNumLab = (TextBMFont*)seekNodeByName(root, "searchNumLab");
    searchNumLab->setString(searchNumStr);

    auto swapBtn = (Button*)seekNodeByName(root, "swapBtn");
    swapBtn->addTouchEventListener(CC_CALLBACK_2(AimDialog::btn_callBack, this));
    
    
    auto bombBtn = (Button*)seekNodeByName(root, "bombBtn");
    bombBtn->addTouchEventListener(CC_CALLBACK_2(AimDialog::btn_callBack, this));
    
    auto searchBtn = (Button*)seekNodeByName(root, "searchBtn");
    searchBtn->addTouchEventListener(CC_CALLBACK_2(AimDialog::btn_callBack, this));

    auto playBtn = (Button*)seekNodeByName(root, "playBtn");
    playBtn->addTouchEventListener(CC_CALLBACK_2(AimDialog::btn_callBack, this));

    auto closeBtn = (Button*)seekNodeByName(root, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(AimDialog::btn_callBack, this));
}

void AimDialog::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    
    if(type == Widget::TouchEventType::ENDED)
    {
        log("btn_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        switch (tag)
        {
            case 1:
                log("swap_callBack");
                break;
            case 2:
                log("bomb_callBack");
                break;
            case 3:
                log("search_callBack");
                break;
            case 4:
                log("start_callBack");
                {
                    auto sc = GameLayer::scene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                }
                break;
            case 5:
                log("close_callBack");
                {
                    removeFromParentAndCleanup(true);
                }
                break;
            default:
                break;
        }
    }
    
}
