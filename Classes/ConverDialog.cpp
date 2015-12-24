//
//  ConverDialog.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#include "ConverDialog.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
#include "ShopLayer.h"
Scene* ConverDialog::scene()
{
    auto sc = Scene::create();
    auto layer =  ConverDialog::create();
    sc->addChild(layer);
    return sc;
}

bool ConverDialog::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void ConverDialog::initData()
{
}

void ConverDialog::initView()
{
    auto rootNode =  CSLoader::createNode("ConVertLayer.csb");
    this->addChild(rootNode);
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ConverDialog::close_callBack, this));
    
    
    auto confirmBtn = (Button*)seekNodeByName(rootNode, "confirmBtn");
    confirmBtn->addTouchEventListener(CC_CALLBACK_2(ConverDialog::confirm_callBack, this));
    
    inputText = (TextField*)seekNodeByName(rootNode, "inputText");
    inputText->addEventListener(CC_CALLBACK_2(ConverDialog::textFieldEvent, this));
    
    msgLab = (Text*)seekNodeByName(rootNode, "msgLab");
    
    bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}


void ConverDialog::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
    }
}

void ConverDialog::confirm_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        int num = atoi(inputText->getString().c_str());
        int coinNum = GameData::getInstance()->getCoinNum();
        int diamondNum = GameData::getInstance()->getDiamondNum();
        int leftCoinNum = coinNum - num * 100;
        if(leftCoinNum >= 0)
        {
            msgLab->setString("兑换成功");
            GameData::getInstance()->setCoinNum(leftCoinNum);
            GameData::getInstance()->setDiamondNum(diamondNum + num);
            auto shopLayer = (ShopLayer*)getParent();
            shopLayer->updateCoinAndDiamondNum();
        }
        else
        {
            msgLab->setString("金币不足");
        }
    }
}

void ConverDialog::textFieldEvent(Ref *pSender, TextField::EventType type)
{
    switch (type)
    {
        case TextField::EventType::ATTACH_WITH_IME:
        {
            log("attach with IME");
            auto moveBy = MoveBy::create(0.1f, Vec2(0,50));
            bg->runAction(moveBy);
        }
            break;
            
        case TextField::EventType::DETACH_WITH_IME:
        {
            auto moveBy = MoveBy::create(0.1f, Vec2(0,-50));
            bg->runAction(moveBy);
            log("detach with IME");
        }
            break;
            
        case TextField::EventType::INSERT_TEXT:
        {
            log("insert words");
            TextField* textField = dynamic_cast<TextField*>(pSender);
//            if(textField)
            {
                std::string str = textField->getString();
                int num = atoi(str.c_str());
                const char* numStr = String::createWithFormat("%d",num)->getCString();
                textField->setString(numStr);
            }
        }
            break;
            
        case TextField::EventType::DELETE_BACKWARD:
        {
            log("delete word");
        }
            break;
            
        default:
            break;
    }
}