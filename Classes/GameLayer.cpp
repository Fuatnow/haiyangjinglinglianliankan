#include "GameLayer.h"
#include "SuccessDialog.h"
#include "PauseDialog.h"
#include "LoseDialog.h"
#include "Resounce.h"
#include "Audio.h"
#include "Tools.h"
#include "Toast.h"
#include "Platform.h"
Scene* GameLayer::scene()
{
	auto sc = Scene::create();
	auto layer = GameLayer::create();
	sc->addChild(layer);
	return sc;
}

bool GameLayer::init()
{
	BasicLayer::init();
	initData();
	initView();
	return true;
}

void GameLayer::initData()
{
    gameState = Game_Play;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cell.plist");
    int level = GameData::getInstance()->getLevel();
//    levelManager->loadLevel(level);
	levelManager = LevelManager::create();
    
    MapInfo info = GameData::getInstance()->getMapInfo();
    levelManager->gainMapInfo(info,level);
    GameData::getInstance()->setMapInfo(info);
	addChild(levelManager);
    
    const char* bgMicFile = String::createWithFormat("chap%d.mp3",info.bg)->getCString();
    if(IS_MARKET_VERSION)
    {
        bgMicFile = "chap1.mp3";
    }
    Audio::getInstance()->playMusic(bgMicFile,true);
    
    setKeyboardEnabled(true);
    //注册捕捉监听
//    auto listenerkeyPad = EventListenerKeyboard::create();
//    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    // 进入关卡.
    const char* levelStr = String::createWithFormat("level-%03d",level)->getCString();
    umeng::MobClickCpp::startLevel(levelStr);
}

void GameLayer::initView()
{
	backgroundLayer = BackgroundLayer::create();
	addChild(backgroundLayer,-1);
    
    board = Board::create();
    addChild(board,99);

    
    menuLayer = MenuLayer::create(this);
    addChild(menuLayer,100);
}

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state)
    {
        case C2DXResponseStateSuccess:
            log("分享成功");
        {
            MessageBox("分享成功系统送你10个钻石,您可以到商店购买道具","恭喜");
            int addDiamondNum = 10;
            int diamond = GameData::getInstance()->getDiamondNum();
            GameData::getInstance()->setDiamondNum(diamond + addDiamondNum);
            GameData::getInstance()->setHadGiveUpdateAward(true);
        }
            break;
        case C2DXResponseStateFail:
            log("分享失败");
            break;
        default:
            break;
    }
}


void GameLayer::initShareBtn(Node* par , bool canReputation)
{
    vector<Node*> btnVec;
    
    //注册
    auto registerBtn = Button::create("btn_blue_nor.png","btn_blue_sel.png","btn_blue_nor.png");
    registerBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::registerCallBack, this));
    registerBtn->setAnchorPoint(Vec2(1,0.47));
//    registerBtn->setPosition(Vec2(0.995f*winWidth,0.42f*winHeight));
    registerBtn->setScale(0.75f,0.8f);
    auto registerLabel = Text::create("游戏注册", "normal.TTF",30);
    registerLabel->setPosition(registerBtn->getContentSize()/2);
    registerBtn->addChild(registerLabel);
    
    auto label = Text::create("注册参与排行榜", "normal.TTF",27);
    label->setAnchorPoint(Vec2(0,1));
    label->setColor(Color3B(255,200,29));
    registerBtn->addChild(label);
    if(canReputation && Platform::getInstance()->isLogin() == false && GameData::getInstance()->getMaxLevel() > 5)
    {
        this->addChild(registerBtn,100);
        btnVec.push_back(registerBtn);
    }
    
    
    
    //分享
    auto shareBtn = Button::create("btn_blue_nor.png","btn_blue_sel.png","btn_blue_nor.png");
    shareBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::shareCallBack, this));
    shareBtn->setAnchorPoint(Vec2(1,0.47));
//    shareBtn->setPosition(Vec2(0.995f*winWidth,0.58f*winHeight));
    shareBtn->setScale(0.75f,0.8f);
    auto sharedLabel = Text::create("游戏分享", "normal.TTF",30);
    sharedLabel->setPosition(shareBtn->getContentSize()/2);
    shareBtn->addChild(sharedLabel);
    this->addChild(shareBtn,100);
    
    label = Text::create("分享送10个钻石", "normal.TTF",27);
    label->setAnchorPoint(Vec2(0,1));
    label->setColor(Color3B(255,200,29));
    shareBtn->addChild(label);
    btnVec.push_back(shareBtn);
    
    //反馈
    auto feedBackBtn = Button::create("btn_blue_nor.png","btn_blue_sel.png","btn_blue_nor.png");
    feedBackBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::feedBackCallBack, this));
    feedBackBtn->setAnchorPoint(Vec2(1,0.47));
//    feedBackBtn->setPosition(Vec2(0.995f*winWidth,0.42f*winHeight));
    feedBackBtn->setScale(0.75f,0.8f);
    auto feedBackLabel = Text::create("游戏建议", "normal.TTF",30);
    feedBackLabel->setPosition(feedBackBtn->getContentSize()/2);
    feedBackBtn->addChild(feedBackLabel);
    this->addChild(feedBackBtn,100);
    
    label = Text::create("采纳送15个钻石", "normal.TTF",27);
    label->setAnchorPoint(Vec2(0,1));
    label->setColor(Color3B(255,200,29));
    feedBackBtn->addChild(label);
    btnVec.push_back(feedBackBtn);
    
    
    //好评
    auto reputationBtn = Button::create("btn_blue_nor.png","btn_blue_sel.png","btn_blue_nor.png");
    reputationBtn->addTouchEventListener(CC_CALLBACK_2(GameLayer::reputationCallBack, this));
    reputationBtn->setAnchorPoint(Vec2(1,0.47));
//    reputationBtn->setPosition(Vec2(0.995f*winWidth,0.42f*winHeight));
    reputationBtn->setScale(0.75f,0.8f);
    auto reputationLabel = Text::create("游戏评价", "normal.TTF",30);
    reputationLabel->setPosition(reputationBtn->getContentSize()/2);
    reputationBtn->addChild(reputationLabel);
    
    label = Text::create("好评支持作者", "normal.TTF",27);
    label->setAnchorPoint(Vec2(0,1));
    label->setColor(Color3B(255,200,29));
    reputationBtn->addChild(label);
    
    if(GameData::getInstance()->getMaxLevel() > 25)
    {
        this->addChild(reputationBtn,100);
        btnVec.push_back(reputationBtn);
    }
    
    
    int size = btnVec.size();
    float posX = 0.995f*winWidth;
    float spaceY = 0.15f*winHeight;
    int offSetY = size / 2 * spaceY + winHeight/2;
    if(size % 2 == 0) offSetY -= spaceY/2;
    for (int i=0; i < size; i++)
    {
        float posY = offSetY - i * spaceY;
        btnVec[i]->setPosition(Vec2(posX,posY));
    }
}
void GameLayer::registerCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Platform::getInstance()->platform(SHOW_LOGINVIEW);
    }
}


void GameLayer::shareCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        //截屏
        utils::captureScreen(CC_CALLBACK_2(GameLayer::capture, this), "screenshot.jpg");
    }
}

void GameLayer::feedBackCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value pay(rapidjson::kObjectType);
        rapidjson::Value cmd(rapidjson::kObjectType);
        rapidjson::Value info(rapidjson::kArrayType);
        int level = GameData::getInstance()->getLevel();
        bool isNeedForeUpdate = GameData::getInstance()->getHaveNeedForeUpdateAPP();
        const char* curLevelStr = String::createWithFormat("%d",level)->getCString();
        cmd.AddMember("cmdId",cmd_feedback, allocator);
        pay.AddMember("curLevel", curLevelStr, allocator);
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
}


void GameLayer::reputationCallBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        getReputation();
    }
}

void GameLayer::capture(bool succeed, const std::string& outputFile)
{
    if (succeed)
    {
        log("%s", outputFile.c_str());
        int level = GameData::getInstance()->getLevel();
        const char* contentStr = CCString::createWithFormat("我在玩第%d关,让我们一起回味经典吧。",level)->getCString();
        CCDictionary *content = CCDictionary::create();
        content -> setObject(CCString::create(contentStr), "content");
//        content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
        content -> setObject(CCString::create(outputFile.c_str()), "image");
        content -> setObject(CCString::create("一起玩"), "title");
        content -> setObject(CCString::create("经典好玩的游戏，根本停不下来"), "description");
        content -> setObject(CCString::create("http://a.app.qq.com/o/simple.jsp?pkgname=com.fuatnow.game.sealink"), "url");
        content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
        content -> setObject(CCString::create("	http://fuatnow.bmob.cn"), "siteUrl");
        content -> setObject(CCString::create("海洋精灵连连看"), "site");
        //        content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
        content -> setObject(CCString::create("extInfo"), "extInfo");
        
        C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
    }
}

void GameLayer::gameOver()
{
    Audio::getInstance()->stopAllEffects();
    int level = GameData::getInstance()->getLevel();
    int maxLevel = GameData::getInstance()->getMaxLevel();
    if(gameState == Game_Success)
    {
        log("game_win");
        float leftTime  = menuLayer->getLeftTime();
        float aimPassTime = GameData::getInstance()->getMapInfo().aimpassInfo.lenTime;
        float perLeft = leftTime / aimPassTime;
        int starNum = perLeft * 5;
        int score = menuLayer->getScoreNum();
        //分数加成
        int extralScore = leftTime * 100;
        score = score + extralScore;
        GameData::getInstance()->setCurLevelStar(starNum);
        GameData::getInstance()->setCurLevelScore(score);
        const char* scoreStr = String::createWithFormat("%d",score)->getCString();
        Platform::getInstance()->platform(REPORT_SCORE,level,scoreStr);
        Audio::getInstance()->playEffect(sound_gamewin);
        auto dialog =  SuccessDialog::create();
        addChild(dialog,100);
        
        // 完成关卡.
        const char* levelStr = String::createWithFormat("level-%03d",level)->getCString();
        umeng::MobClickCpp::finishLevel(levelStr);
    }
    else if(gameState == Game_Lose)
    {
        log("game_lose");
        Audio::getInstance()->playEffect(sound_gamelose);
        auto dialog =  LoseDialog::create();
        addChild(dialog,100);
        
        // 失败关卡.
        const char* levelStr = String::createWithFormat("level-%03d",level)->getCString();
        umeng::MobClickCpp::failLevel(levelStr);
    }


    const char* maxLevelStr = String::createWithFormat("level-%03d",maxLevel)->getCString();
    umeng::MobClickCpp::setUserLevel(maxLevelStr);
    
    menuLayer->setDisable();
    reportTotalScore();

    bool isShowAd = false;
    if(gameState == Game_Success && (level == 5 || level%15 == 0))
    {
        //如果当前关卡没有点击过好评
        if(GameData::getInstance()->getPreClickReputationLevel() != level)
        {
            getReputation();
            GameData::getInstance()->setPreClickReputationLevel(level);
        }
    }
    else if(level >4 && rand_0_1() < 0.74f)
    {
        loadAD();
        isShowAd = true;
    }
    
    
    initShareBtn(this,!isShowAd);
}

void GameLayer::loadAD()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value pay(rapidjson::kObjectType);
    rapidjson::Value cmd(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kArrayType);
    int maxLevel = GameData::getInstance()->getMaxLevel();
    bool isNeedForeUpdate = GameData::getInstance()->getHaveNeedForeUpdateAPP();
    const char* maxLevelStr = String::createWithFormat("%d",maxLevel)->getCString();
    const char* needForeUpdate = String::createWithFormat("%d",isNeedForeUpdate)->getCString();
    cmd.AddMember("cmdId",cmd_loadAD, allocator);
    pay.AddMember("adType", "ZhangYun", allocator);
    pay.AddMember("maxLevel", maxLevelStr, allocator);
    pay.AddMember("needForeUpdate", needForeUpdate, allocator);
//    pay.AddMember("", "", allocator);
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


void GameLayer::getReputation()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value pay(rapidjson::kObjectType);
    rapidjson::Value cmd(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kArrayType);
    int level = GameData::getInstance()->getLevel();
    bool isNeedForeUpdate = GameData::getInstance()->getHaveNeedForeUpdateAPP();
    const char* curLevelStr = String::createWithFormat("%d",level)->getCString();
    cmd.AddMember("cmdId",cmd_reputation, allocator);
    pay.AddMember("curLevel", curLevelStr, allocator);
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

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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

void GameLayer::reportTotalScore()
{
    int totalScore = GameData::getInstance()->getTotalScore();
    log("totalScore:%d",totalScore);
    const char* scoreStr = String::createWithFormat("%d",totalScore)->getCString();
    Platform::getInstance()->platform(REPORT_SCORE,0,scoreStr);
}

void GameLayer::pauseGame()
{
    gameState = Game_Pause;
    auto pauseDialog = PauseDialog::create();
    addChild(pauseDialog,100);
    operateAllSchedulerAndActions(this,k_Operate_Pause);
    operateAllSchedulerAndActions(pauseDialog,k_Operate_Resume);
}


void GameLayer::resumeGame()
{
    gameState = Game_Play;
    operateAllSchedulerAndActions(this,k_Operate_Resume);
}

