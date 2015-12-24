//
//  BuyTipDialog.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#include "BuyTipDialog.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
#include "ShopLayer.h"
#define Buy_Kinds 4
Scene* BuyTipDialog::scene()
{
    auto sc = Scene::create();
    auto layer =  BuyTipDialog::create();
    sc->addChild(layer);
    return sc;
}

bool BuyTipDialog::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void BuyTipDialog::initData()
{
    //    diamondVec.resize(Buy_Kinds);
    //    monyVec.resize(Buy_Kinds);
    
    diamondVec.push_back(10);
    monyVec.push_back(1);
    diamondVec.push_back(60);
    monyVec.push_back(5);
    diamondVec.push_back(130);
    monyVec.push_back(10);
    diamondVec.push_back(450);
    monyVec.push_back(30);
}

void BuyTipDialog::initView()
{
    auto rootNode =  CSLoader::createNode("BuyTipDialog.csb");
    this->addChild(rootNode);
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(BuyTipDialog::btn_callBack, this));
    
    auto cancelBtn = (Button*)seekNodeByName(rootNode, "cancelBtn");
    cancelBtn->addTouchEventListener(CC_CALLBACK_2(BuyTipDialog::btn_callBack, this));
    
    auto okBtn = (Button*)seekNodeByName(rootNode, "okBtn");
    okBtn->addTouchEventListener(CC_CALLBACK_2(BuyTipDialog::btn_callBack, this));
    
    auto bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}


void BuyTipDialog::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        switch (tag)
        {
            case 0:
            case 1:
            {
                removeFromParent();
            }
                break;
            case 2:
            {
                //to do something
                ShopLayer::getInstance()->buyPropSuccess();
                removeFromParent();
            }
                break;
                default:
                break;
        }
    }
}