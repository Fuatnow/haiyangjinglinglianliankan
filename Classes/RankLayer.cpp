//
//  RankLayer.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#include "RankLayer.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
static RankLayer* instance = NULL;

RankLayer::~RankLayer()
{
    log("~RankLayer");
}


RankLayer* RankLayer::getInstance()
{
    return instance;
}

Scene* RankLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  RankLayer::create();
    sc->addChild(layer);
    return sc;
}

bool RankLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void RankLayer::initData()
{
    instance = this;
}

void RankLayer::initView()
{
    auto rootNode =  CSLoader::createNode("RankLayer.csb");
    this->addChild(rootNode);
 
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::close_callBack, this));
    
    playerBtn = (Button*)seekNodeByName(rootNode, "playerBtn");
    playerBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::type_callBack, this));
    playerSelBtn = (Button*)seekNodeByName(rootNode, "playerSelBtn");
    playerSelBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::type_callBack, this));
    friendBtn = (Button*)seekNodeByName(rootNode, "friendBtn");
    friendBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::type_callBack, this));
    friendSelBtn = (Button*)seekNodeByName(rootNode, "friendSelBtn");
    friendSelBtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::type_callBack, this));
    
    playerScView = (ScrollView*)seekNodeByName(rootNode, "playerScView");
    friendScView = (ScrollView*)seekNodeByName(rootNode, "friendScView");
    
    bg = seekNodeByName(rootNode, "bg");
    appear();
}

void RankLayer::loadLeaderboard(int boardID)
{
    Platform::getInstance()->platform(FRIEND_LEADER_BOARD, boardID);
    Platform::getInstance()->platform(GLOBAL_LEADER_BOARD, boardID);
}

void RankLayer::initPlayerView()
{
    playerBtn->setVisible(true);
    playerSelBtn->setVisible(false);
    
    friendBtn->setVisible(true);
    friendSelBtn->setVisible(false);
    
    friendScView->setVisible(false);
    playerScView->setVisible(true);
}

void RankLayer::initFriendView()
{
    friendBtn->setVisible(false);
    friendSelBtn->setVisible(true);
    
    playerBtn->setVisible(false);
    playerSelBtn->setVisible(true);
    
    playerScView->setVisible(false);
    friendScView->setVisible(true);
}

void RankLayer::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
       // setVisible(false);
    }
}

void RankLayer::type_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        switch (tag)
        {
            case 0:
                log("playerBtn");
                {
                    initPlayerView();
                }
                break;
            case 3:
                log("friendBtn");
                {
                    initFriendView();
                }
                break;
            default:
                break;
        }
    }
}

void RankLayer::appear()
{
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
    setVisible(true);
}

void RankLayer::onExit()
{
    log("RankLayer onExit");
    BasicLayer::onExit();
    auto playerChildren = playerScView->getInnerContainer()->getChildren();
    for (auto child : playerChildren)
    {
        child->removeFromParentAndCleanup(false);
    }
    playerChildren.clear();
    
    auto friendChildren = friendScView->getInnerContainer()->getChildren();
    for (auto child : friendChildren)
    {
        child->removeFromParentAndCleanup(false);
    }
    friendChildren.clear();
}