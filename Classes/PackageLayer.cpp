//
//  PackageLayer.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#include "PackageLayer.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
Scene* PackageLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  PackageLayer::create();
    sc->addChild(layer);
    return sc;
}

bool PackageLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void PackageLayer::initData()
{
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shopLayer.plist");
}

void PackageLayer::initView()
{
    auto rootNode =  CSLoader::createNode("PackageLayer.csb");
    this->addChild(rootNode);
    
    int propKindsNum = GameData::getInstance()->getPropKindsNum();
    vector<int> kindNums = GameData::getInstance()->getPropNums();
    for(int i=1; i<=propKindsNum;i++)
    {
        const char* str = String::createWithFormat("shop_goods_text_%02d",i)->getCString();
        auto numLabel = (TextBMFont*)seekNodeByName(rootNode, str);
        str = String::createWithFormat("x%d",kindNums[i-1])->getCString();
        numLabel->setString(str);
    }
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(PackageLayer::close_callBack, this));
    
    
    auto bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}


void PackageLayer::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
    }
}