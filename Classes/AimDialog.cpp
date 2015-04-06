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
    
    Node* root = CSLoader::createNode("LevelAimDialog.csb");
    auto size = Director::getInstance()->getWinSize();
    root->setPosition(size/2);
    this->addChild(root);
    
    int level = GameData::getInstance()->getLevel();
    auto levelLabel = (Text*)seekNodeByName(root, "levelLabel");
    std::stringstream ss;
    ss << level;
    levelLabel->setString(ss.str());
    
    auto playerBestLabel = (TextBMFont*)seekNodeByName(root, "playerBestLabel");
    playerBestLabel->setString("0");
    
    auto selfBestLabel = (TextBMFont*)seekNodeByName(root, "selfBestLabel");
    selfBestLabel->setString("0");
    
    auto swapNumLab = (TextBMFont*)seekNodeByName(root, "swapNumLab");
    swapNumLab->setString("x3");
    auto bombNumLab = (TextBMFont*)seekNodeByName(root, "bombNumLab");
    bombNumLab->setString("x2");
    auto searchNumLab = (TextBMFont*)seekNodeByName(root, "searchNumLab");
    searchNumLab->setString("x1");

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
        CCLOG("btn_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        switch (tag)
        {
            case 1:
                CCLOG("swap_callBack");
                break;
            case 2:
                CCLOG("bomb_callBack");
                break;
            case 3:
                CCLOG("search_callBack");
                break;
            case 4:
                CCLOG("start_callBack");
                {
                    auto sc = GameLayer::scene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
                }
                break;
            case 5:
                CCLOG("close_callBack");
                {
                    removeFromParentAndCleanup(true);
                }
                break;
            default:
                break;
        }
    }
    
}
