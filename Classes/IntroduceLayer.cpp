//
//  IntroduceLayer.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#include "IntroduceLayer.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
Scene* IntroduceLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  IntroduceLayer::create();
    sc->addChild(layer);
    return sc;
}

bool IntroduceLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void IntroduceLayer::initData()
{
}

void IntroduceLayer::initView()
{
    auto rootNode =  CSLoader::createNode("IntrasitionLayer.csb");
    this->addChild(rootNode);
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(IntroduceLayer::close_callBack, this));
    
    auto bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}


void IntroduceLayer::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
    }
}
