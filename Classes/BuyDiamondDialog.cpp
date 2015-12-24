//
//  BuyDiamondDialog.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#include "BuyDiamondDialog.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
#include "ShopLayer.h"
#define Buy_Kinds 4
Scene* BuyDiamondDialog::scene()
{
    auto sc = Scene::create();
    auto layer =  BuyDiamondDialog::create();
    sc->addChild(layer);
    return sc;
}

bool BuyDiamondDialog::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void BuyDiamondDialog::initData()
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

void BuyDiamondDialog::initView()
{
    auto rootNode =  CSLoader::createNode("BuyDiamondDialog.csb");
    this->addChild(rootNode);
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(BuyDiamondDialog::close_callBack, this));
    
    for(int i=1 ; i<=Buy_Kinds;i++)
    {
        const char* str = String::createWithFormat("item%d",i)->getCString();
        auto img = (ImageView*)seekNodeByName(rootNode, str);
        img->addTouchEventListener(CC_CALLBACK_2(BuyDiamondDialog::img_callBack, this));
    }
    
    auto bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}


void BuyDiamondDialog::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
    }
}


void BuyDiamondDialog::img_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto node = (Node*)pSender;
        int tag = node->getTag();
        int diamondNum = diamondVec[tag-1];
        int monyNum = monyVec[tag-1];
        log("diamondNum:%d monyNum:%d",diamondNum,monyNum);
        
        const char* buyInfo = String::createWithFormat("你需要支付%d元购买%d个钻石",monyNum,diamondNum)->getCString();
        
        const char* price = String::createWithFormat("%d",monyNum)->getCString();
        const char* payId = String::createWithFormat("%d",tag)->getCString();
        
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value pay(rapidjson::kObjectType);
        rapidjson::Value cmd(rapidjson::kObjectType);
        rapidjson::Value info(rapidjson::kArrayType);
        //        const char* cmdStr = String::createWithFormat("%d",(int)LOGIN)->getCString();
       
        cmd.AddMember("cmdId", cmd_pay, allocator);
        
        pay.AddMember("price", price, allocator);
        pay.AddMember("name", "购买钻石", allocator);
        pay.AddMember("body", buyInfo, allocator);
        pay.AddMember("payId", payId, allocator);
        info.PushBack(cmd, allocator);
        info.PushBack(pay, allocator);
        document.AddMember("info", info, allocator);
        
        StringBuffer buffer;
        rapidjson::Writer<StringBuffer> writer(buffer);
        document.Accept(writer);
        log("%s",buffer.GetString());
        
        string jsonstr = StringUtils::format("%s",buffer.GetString());
        string rtnStr = Platform::getInstance()->platform(jsonstr);
        log("returnStr:%s",rtnStr.c_str());
        
        //Platform::getInstance()->platform(SHOW_KTPLAY, 0);
        //Platform::getInstance()->platform(DISPATCH_REWARD, 0);
        
    }
}