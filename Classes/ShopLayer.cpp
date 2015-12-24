//
//  ShopLayer.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#include "ShopLayer.h"
#include "GameData.h"
#include "Audio.h"
#include "Define.h"
#include "Platform.h"
#include "IntroduceLayer.h"
#include "ConverDialog.h"
#include "BuyDiamondDialog.h"
#include "BuyTipDialog.h"
#include "Toast.h"
#define Goods_Nums 8

static ShopLayer* instance= NULL;

ShopLayer* ShopLayer::getInstance()
{
    instance =  instance ? instance : ShopLayer::create();
    return instance;
}

Scene* ShopLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  ShopLayer::create();
    sc->addChild(layer);
    return sc;
}

bool ShopLayer::init()
{
    BasicLayer::init();
    instance = this;
    initData();
    initView();
    return true;
}

void ShopLayer::initData()
{
//    diamondVec.resize(Goods_Nums);
    diamondVec.push_back(5);
    diamondVec.push_back(6);
    diamondVec.push_back(8);
    diamondVec.push_back(10);
    diamondVec.push_back(20);
    diamondVec.push_back(24);
    diamondVec.push_back(32);
    diamondVec.push_back(40);
    
//    diamondInfo.resize(Goods_Nums);
    diamondInfo.push_back("[重排](1个)");
    diamondInfo.push_back("[自动消除](1个)");
    diamondInfo.push_back("[炸弹](1个)");
    diamondInfo.push_back("[加时](1个)");
    diamondInfo.push_back("[重排](5个)");
    diamondInfo.push_back("[自动消除](5个)");
    diamondInfo.push_back("[炸弹](5个)");
    diamondInfo.push_back("[加时](5个)");
    
    //购买商品的ID
    payPropId = -1;
}

void ShopLayer::initView()
{
    auto rootNode =  CSLoader::createNode("ShopLayer.csb");
    this->addChild(rootNode);
    
    diamonLab = (TextBMFont*)seekNodeByName(rootNode, "diamonLab");
    coinLab = (TextBMFont*)seekNodeByName(rootNode, "coinLab");
    
    auto closeBtn = (Button*)seekNodeByName(rootNode, "closeBtn");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::close_callBack, this));

    auto helpBtn = (Button*)seekNodeByName(rootNode, "helpBtn");
    helpBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::help_callBack, this));
    auto addBtn = (Button*)seekNodeByName(rootNode, "addBtn");
    addBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::add_callBack, this));
    if(IS_MARKET_VERSION) addBtn->setVisible(false);
    auto conBtn = (Button*)seekNodeByName(rootNode, "conBtn");
    conBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::con_callBack, this));
    
    for(int i=1 ; i<=Goods_Nums;i++)
    {
        const char* str = String::createWithFormat("item%d",i)->getCString();
        auto btn = (Button*)seekNodeByName(rootNode, str);
        btn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::btn_callBack, this));
    }
    
    updateCoinAndDiamondNum();
    
    auto bg = seekNodeByName(rootNode, "bg");
    auto pos = bg->getPosition();
    bg->setPosition(Vec2(winWidth + bg->getContentSize().width/2,pos.y));
    auto moveTo = MoveTo::create(0.3f,pos + Vec2(-0.03f*winWidth,0));
    auto bounsAction = EaseIn::create(moveTo,0.2f);
    auto moveBy = MoveBy::create(0.05f, Vec2(0.03f*winWidth,0));
    auto seq = Sequence::create(bounsAction, moveBy,NULL);
    bg->runAction(seq);
}

void ShopLayer::updateCoinAndDiamondNum()
{
    int coinNum = GameData::getInstance()->getCoinNum();
    int diamondNum = GameData::getInstance()->getDiamondNum();
    const char* str = String::createWithFormat("%d",coinNum)->getCString();
    coinLab->setString(str);
    str = String::createWithFormat("%d",diamondNum)->getCString();
    diamonLab->setString(str);
}

void ShopLayer::close_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        removeFromParent();
    }
}

void ShopLayer::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        payPropId = tag - 1;
        int needDiamond = diamondVec[payPropId];
        std::string title = diamondInfo[payPropId];
        log("needDiamond:%d title:%s",needDiamond,title.c_str());
        
        int diamondNum = GameData::getInstance()->getDiamondNum();
        if(diamondNum < needDiamond)
        {
            if(IS_MARKET_VERSION == false)
            {
                auto dialog = BuyDiamondDialog::create();
                addChild(dialog,10);
                Toast::toast("您的钻石数量不足,请充值");
            }
            else
            {
                Toast::toast("您的钻石数量不足,请用金币兑换");
            }
        }
        else
        {
            auto dialog = BuyTipDialog::create();
            addChild(dialog,10);
        }
    }
}

void ShopLayer:: help_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto layer = IntroduceLayer::create();
        addChild(layer);
    }
}

void ShopLayer:: add_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto layer = BuyDiamondDialog::create();
        addChild(layer);
    }
}
void ShopLayer::con_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto layer = ConverDialog::create();
        addChild(layer);
    }
}


void ShopLayer::payInfo(std::string jsonStr)
{
    log("%s",jsonStr.c_str());
    
    Document doc;///<创建一个Document对象rapidJson的相关操作都在Document类中
    doc.Parse<0>(jsonStr.c_str());///<通过Parse方法将Json数据解析出来
    if(doc.HasParseError())
    {
        log("GetParseError%s\n",doc.GetParseError());
        return;
    }
    
    const rapidjson::Value &info = doc["info"];
    const char* payIdStr = info["payId"].GetString();
    const char* statusStr = info["status"].GetString();
    std::string orderId = info["orderId"].GetString();
    int payId = atoi(payIdStr);
    int status = atoi(statusStr);
    log("payId:%d status:%d orderId:%s",payId,status,orderId.c_str());
    switch (status)
    {
        case 0:
            //fail
            break;
        case 1:
            giveDiamond(payId);
            //suc
            break;
        case 2:
            //unknown
            break;
        default:
            break;
    }
}

void ShopLayer::giveDiamond(int payId)
{
    int diamond = 0;
    float money = 0;
    switch (payId)
    {
        case 1:
            diamond = 10;
            money = 1;
            break;
        case 2:
            diamond = 60;
            money = 5;
            break;
        case 3:
            diamond = 130;
            money = 10;
            break;
        case 4:
            diamond = 450;
            money = 30;
            break;
        default:
            break;
    }
    const char* infoStr = String::createWithFormat("恭喜你,支付成功你获得了%d个钻石",diamond)->getCString();
    
    int diamondNum = GameData::getInstance()->getDiamondNum();
    GameData::getInstance()->setDiamondNum(diamondNum+diamond);
    
    updateCoinAndDiamondNum();
    
    //花了xx元买了xx个钻石
    umeng::MobClickCpp::pay(money, 2, diamond);
    
    MessageBox(infoStr, "支付完成");
}

void ShopLayer::buyPropSuccess()
{
    switch (payPropId)
    {
        case 0:
        {
            int swapNum = GameData::getInstance()->getSearchNum()+1;
            GameData::getInstance()->setSwapNum(swapNum);
        }
            break;
        case 1:
        {
            int searchNum = GameData::getInstance()->getSearchNum()+1;
            GameData::getInstance()->setSearchNum(searchNum);
        }
            break;
        case 2:
        {
            int bombNum = GameData::getInstance()->getBombNum()+1;
            GameData::getInstance()->setBombNum(bombNum+1);
        }
            break;
        case 3:
        {
            int timeNum = GameData::getInstance()->getTimeNum()+1;
            GameData::getInstance()->setTimeNum(timeNum);
        }
            break;
        case 4:
        {
            int swapNum = GameData::getInstance()->getSearchNum()+1;
            GameData::getInstance()->setSwapNum(swapNum);
        }
            break;
        case 5:
        {
            int searchNum = GameData::getInstance()->getSearchNum()+5;
            GameData::getInstance()->setSearchNum(searchNum);
        }
            break;
        case 6:
        {
            int bombNum = GameData::getInstance()->getBombNum()+5;
            GameData::getInstance()->setBombNum(bombNum+1);
        }
            break;
        case 7:
        {
            int timeNum = GameData::getInstance()->getTimeNum()+5;
            GameData::getInstance()->setTimeNum(timeNum);
        }
            break;
        default:
            break;
    }
    
    int diamond = GameData::getInstance()->getDiamondNum();
    diamond = diamond - diamondVec[payPropId];
    GameData::getInstance()->setDiamondNum(diamond);
    
    updateCoinAndDiamondNum();
    
    std::string title = diamondInfo[payPropId];
    log("%s",title.c_str());
    title = "您已经获得道具" + title + ",请到背包查看";
    log("%s",title.c_str());
    Toast::toast(title);
    //购买diamondInfo[payPropId]道具花了diamondVec[payPropId]个钻石
    umeng::MobClickCpp::buy(diamondInfo[payPropId].c_str(), 1, diamondVec[payPropId]);
    
    payPropId = -1;
}
