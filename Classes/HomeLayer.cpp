//
//  HomeLayer.cpp
//  Link
//
//  Created by Fuatnow on 15-3-27.
//
//

#include "HomeLayer.h"
#include "LevelLayer.h"
#include "Audio.h"
#include "Define.h"
#include "GlobalNode.h"
#include "Platform.h"
#include "GameData.h"
#include "Toast.h"

static HomeLayer* instance = NULL;

HomeLayer* HomeLayer::getInstance()
{
    instance =  instance ? instance : HomeLayer::create();
    return instance;
}

Scene* HomeLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  HomeLayer::create();
    sc->addChild(layer);
    return sc;
}

bool HomeLayer::init()
{
    BasicLayer::init();
    instance = this;
    initData();
    initView();
    testHaveNeedDownNewApp();
    return true;
}

void HomeLayer::initData()
{
    Platform::getInstance()->platform(DISPATCH_REWARD, 0);
    Platform::getInstance()->platform(STATUS_CHANGED, 0);
    Platform::getInstance()->platform(AVAILABILITY_CHANGED, 0);
    Platform::getInstance()->platform(DEEP_LINNK, 0);
    Platform::getInstance()->platform(SHOW_INTERSTITIAL_NOTIFICATION, 0);
    Platform::getInstance()->platform(KTPLAY_DIDAPPEAR, 0);
    Platform::getInstance()->platform(KTPLAY_DISAPPEAR, 0);
    Platform::getInstance()->platform(LOGIN_STATUSCHANGED,0);
    auto node = GlobalNode::create();
//    auto notiNode = Director::getInstance()->getNotificationNode();
//    log("err:%d",notiNode->getReferenceCount());
//    notiNode->release();
    Director::getInstance()->setNotificationNode(node);
    
    haveNeedDownNewApp = false;
    isNeedForeUpdate = false;
    hadClickNotUpdate = false;
    
    //setKeyboardEnabled(true);
    //注册捕捉监听
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(HomeLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
}

void HomeLayer::initView()
{
    auto backgroundLayer = BackgroundLayer::create();
    addChild(backgroundLayer,-1);
    
    auto rootNode =  CSLoader::createNode("HomeLayer.csb");
    this->addChild(rootNode);

    startItem = (Button*)seekNodeByName(rootNode, "playButton");
    startItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::start_callBack, this));
    
    
    bool musicCanPlay = Audio::getInstance()->getMusicCanPlay();
    bool soundCanPlay = Audio::getInstance()->getSoundCanPlay();
    
    musicItem = (Button*)seekNodeByName(rootNode, "musicItem_on");
    musicItem->setVisible(musicCanPlay);
    musicItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::music_callBack, this));
    musicItem_off = (Button*)seekNodeByName(rootNode, "musicItem_off");
    musicItem_off->addTouchEventListener(CC_CALLBACK_2(HomeLayer::music_callBack, this));
    musicItem_off->setVisible(!musicCanPlay);
   
    soundItem = (Button*)seekNodeByName(rootNode, "soundItem_on");
    soundItem->setVisible(soundCanPlay);
    soundItem->addTouchEventListener(CC_CALLBACK_2(HomeLayer::sound_callBack, this));
    soundItem_off = (Button*)seekNodeByName(rootNode, "soundItem_off");
    soundItem_off->addTouchEventListener(CC_CALLBACK_2(HomeLayer::sound_callBack, this));
    soundItem_off->setVisible(!soundCanPlay);
    
    Audio::getInstance()->playMusic(DefaultMuisc, true);
}

void HomeLayer::testHaveNeedDownNewApp()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value pay(rapidjson::kObjectType);
    rapidjson::Value cmd(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kArrayType);
    int appVersion = GameData::getInstance()->getAppVersion();
    const char* verStr = String::createWithFormat("%d",appVersion)->getCString();
    const char* isMarketVersion = String::createWithFormat("%d",IS_MARKET_VERSION)->getCString();
    cmd.AddMember("cmdId", down_app, allocator);
    pay.AddMember("version", verStr, allocator);
    pay.AddMember("cloudCmd", down_app, allocator);
    pay.AddMember("isMarketVersion", isMarketVersion, allocator);
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
}


void HomeLayer::start_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
//        rapidjson::Document document;
//        document.SetObject();
//        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//        rapidjson::Value user(rapidjson::kObjectType);
//        rapidjson::Value cmd(rapidjson::kObjectType);
//        rapidjson::Value info(rapidjson::kArrayType);
////        const char* cmdStr = String::createWithFormat("%d",(int)LOGIN)->getCString();
//        const char* cmdStr = "0";
//        cmd.AddMember("cmdId", cmdStr, allocator);
//
//        user.AddMember("userName", "游客", allocator);
//        user.AddMember("password", "123456", allocator);
//
//        info.PushBack(cmd, allocator);
//        info.PushBack(user,allocator);
//
//        document.AddMember("info", info, allocator);
//
//        StringBuffer buffer;
//        rapidjson::Writer<StringBuffer> writer(buffer);
//        document.Accept(writer);
//        log("%s",buffer.GetString());
//
//        string jsonstr = StringUtils::format("%s",buffer.GetString());
//        string rtnStr = Platform::getInstance()->platform(jsonstr);
//        log("returnStr:%s",rtnStr.c_str());
//
//        Platform::getInstance()->platform(SHOW_KTPLAY, 0);
//        Platform::getInstance()->platform(DISPATCH_REWARD, 0);
//
//        MessageBox(rtnStr.c_str(), "test");
//        return;

        log("srat_callBack");
//        testHaveNeedDownNewApp();
//        return;

        //必须强制更新
        if(isNeedForeUpdate)
        {
            bool hadOncePlayGame = GameData::getInstance()->getHadOncePlayGame();
            //是从市场上刚下载的就让强制更新不友好 让他玩两把再强更新
            if(IS_MARKET_VERSION && hadOncePlayGame == false)
            {
                //用户已经玩过了
                GameData::getInstance()->setHadOncePlayGame(true);
            }
            else
            {
                foreUpdateApp();
                Toast::toast("您当前的版本太低了,需要强制更新到最新版",Vec2(winWidth*0.5f,winHeight*0.06f),5);
                return;
            }
        }
        else if(haveNeedDownNewApp && hadClickNotUpdate == false)
        {
            //可以暂不更新
            hadClickNotUpdate = true;
            foreUpdateApp();
            return;
        }
        
        Audio::getInstance()->playEffect(sound_click2);
        auto move = MoveBy::create(0.5, Vec2(-0.3f*winWidth,0));
        musicItem->runAction(move->clone());
        musicItem_off->runAction(move->clone());
        soundItem->runAction(move->reverse()->clone());
        soundItem_off->runAction(move->reverse()->clone());
        auto play = (Node*)pSender;
        auto moveUp = MoveBy::create(0.5f, Vec2(0,0.7f*winHeight));
        //不起作用
        play->stopAllActions();
        //play->runAction(moveUp);
        
        auto sc = LevelLayer::scene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, sc));
    }
}

void HomeLayer::music_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    {
        log("music_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        musicItem->setVisible(!tag);
        musicItem_off->setVisible(tag);
        Audio::getInstance()->playEffect(sound_click);
        Audio::getInstance()->setMusicCanPlay(!tag);
    }
}

void HomeLayer::sound_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    {
        log("sound_callBack");
        Button* btn = (Button*)pSender;
        int tag = btn->getTag();
        soundItem->setVisible(!tag);
        soundItem_off->setVisible(tag);
        Audio::getInstance()->setSoundCanPlay(!tag);
        Audio::getInstance()->playEffect(sound_click);
    }
}


void HomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_BACK:
        {
            rapidjson::Document document;
            document.SetObject();
            rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
            rapidjson::Value pay(rapidjson::kObjectType);
            rapidjson::Value cmd(rapidjson::kObjectType);
            rapidjson::Value info(rapidjson::kArrayType);
            //        const char* cmdStr = String::createWithFormat("%d",(int)LOGIN)->getCString();
            
            cmd.AddMember("cmdId",key_back, allocator);
            pay.AddMember("keyBack", "keyBack", allocator);
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
        }
        default:
            break;
    }
}


void HomeLayer::downNewAppInfo(std::string jsonStr)
{
    log("downNewApp: %s",jsonStr.c_str());
    Document doc;///<创建一个Document对象rapidJson的相关操作都在Document类中
    doc.Parse<0>(jsonStr.c_str());///<通过Parse方法将Json数据解析出来
    if(doc.HasParseError())
    {
        log("GetParseError%s\n",doc.GetParseError());
        return;
    }
    const rapidjson::Value &info = doc["info"];
    const char* needDownStr = info["needDownStr"].GetString();
    const char* foreUpdateStr = info["foreUpdateStr"].GetString();
    const char* diamondNumStr = info["diamondNum"].GetString();
    haveNeedDownNewApp = atoi(needDownStr);
    isNeedForeUpdate = atoi(foreUpdateStr);
    GameData::getInstance()->setHaveNeedForeUpdateAPP(isNeedForeUpdate);
    int addDiamondNum = atoi(diamondNumStr);
    bool hadGiveUpdateAward = GameData::getInstance()->getHadGiveUpdateAward();
    if(isNeedForeUpdate)
    {
        GameData::getInstance()->setHadGiveUpdateAward(false);
    }
    else if(hadGiveUpdateAward == false && IS_MARKET_VERSION == false)
    {
        MessageBox("更新版本系统送你10个钻石","恭喜");
        int diamond = GameData::getInstance()->getDiamondNum();
        GameData::getInstance()->setDiamondNum(diamond + addDiamondNum);
        GameData::getInstance()->setHadGiveUpdateAward(true);
    }

    log("haveNeedDownNewApp:%d isNeedForeUpdate:%d",haveNeedDownNewApp,isNeedForeUpdate);
}

void:: HomeLayer::foreUpdateApp()
{    
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value pay(rapidjson::kObjectType);
    rapidjson::Value cmd(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kArrayType);
    int appVersion = GameData::getInstance()->getAppVersion();
    const char* verStr = String::createWithFormat("%d",appVersion)->getCString();
    
    cmd.AddMember("cmdId", fore_down_app, allocator);
    pay.AddMember("version", verStr, allocator);
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
    
}

void HomeLayer::pareseJsonTest()
{
//    //解析json
//
//    /***
//     {"cmdId":"1","info":{"payId":"3","status":"0","orderId":"3af554732af782421c405a6544eb2ec5"}}
//     */
//    ssize_t size;
//    unsigned char*ch=FileUtils::getInstance()->getFileData("test.json","r",&size);
//    std::string data=std::string((const char*)ch,size);
//    
//    Document doc;///<创建一个Document对象rapidJson的相关操作都在Document类中
//    doc.Parse<0>(data.c_str());///<通过Parse方法将Json数据解析出来
//    if(doc.HasParseError())
//    {
//        log("GetParseError%s\n",doc.GetParseError());
//    }
//    
//    log("%s",data.c_str());
//    
//    
//    rapidjson::Value& cmdValue=doc["cmdId"];
//    rapidjson::Value& infoValue=doc["info"];
//    const char* cmdId = NULL;
//    const char* str = NULL;
//    
//    auto type = infoValue.GetType();
//    
//    if(cmdValue.IsString())
//    {
//        cmdId = cmdValue.GetString();
//    }
    
//    if(infoValue.IsString())
//    {
//        str =infoValue.GetString();
//    }
//    str =infoValue.GetString();
//    log("cmdId:%s info:%s",cmdId,str);
    
    
}


//
//std::string JsonMake::getJsonMyBaodanMain()
//{
//    rapidjson::Document document;
//    document.SetObject();
//    
//    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//    rapidjson::Value object1(rapidjson::kObjectType);
//    rapidjson::Value object2(rapidjson::kObjectType);
//    rapidjson::Value object3(rapidjson::kObjectType);
//    rapidjson::Value object4(rapidjson::kArrayType);
//    
//    object1.AddMember("Name", "Yuxikuo", allocator);
//    object1.AddMember("Age", "18", allocator);
//    object1.AddMember("Sex", "man", allocator);
//    
//    object2.AddMember("Name", "WangDongGe", allocator);
//    object2.AddMember("Age", "32", allocator);
//    object2.AddMember("Sex", "woman", allocator);
//    
//    object3.AddMember("Name", "zhanhong", allocator);
//    object3.AddMember("Age", "20", allocator);
//    object3.AddMember("Sex", "man", allocator);
//    
//    object4.PushBack(object1,allocator);
//    object4.PushBack(object2,allocator);
//    object4.PushBack(object3,allocator);
//    
//    document.AddMember("info", object4, allocator);
//    
//    StringBuffer buffer;
//    rapidjson::Writer<StringBuffer> writer(buffer);
//    document.Accept(writer);
//    log("%s",buffer.GetString());
//    
//    return StringUtils::format("%s",buffer.GetString());
//    
//}


//保存到本地
//auto path = FileUtils::getInstance()->getWritablePath();
//log("path = %s",path.c_str());
//path.append("myBaodanMain.json");
//FILE* fp = std::fopen(path.c_str(), "at+");
//CCASSERT(fp != NULL, "file open error");
//
//auto jsonMake = JsonMake::create();
//auto str = jsonMake->getJsonMyBaodanMain();
//auto length = str.length();
//fwrite(str.c_str(), length, 1 ,fp);
//fclose(fp);
