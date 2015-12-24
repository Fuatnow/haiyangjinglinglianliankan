#include "LevelLayer.h"
#include "math.h"
#include "GameData.h"
#include "AimDialog.h"
#include "Audio.h"
#include "Toast.h"
#include "Platform.h"
#define  HERO_JUMP_HEIGHT 50
#define  MENU_TAG_BASE    100
#define  Pop_Btn_Num      3
#define  Root_Tag         100
Btn_FrameName btnFramNames[CHAPTER_NUMS]=
{
    {"ui-btnyellowegg.png","ui-btnyelloweggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(0, 0, 0)},
    {"ui-btnbuleegg.png","ui-btnbuleeggSelected.png","ui-btnbuleegg.png","ui-btnpurpleeggSelected.png",Color3B(232, 31, 20)},
    {"ui-btngreenegg.png","ui-btngreeneggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(14, 78, 130)},
    {"ui-btnpurpleegg.png","ui-btnpurpleeggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(65, 23, 139)},
    {"ui-btnbuleegg.png","ui-btnbuleeggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(198, 12, 43)},
    {"ui-btngreenegg.png","ui-btngreeneggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(53, 99, 178)}
};

Scene* LevelLayer::scene()
{
	auto sc = Scene::create();
	auto layer = LevelLayer::create();
	sc->addChild(layer);
	return sc;
}

bool LevelLayer::init()
{
	BasicLayer::init();
	initData();
	initView();
	return true;
}


void LevelLayer::initData()
{
    curPageIndex = GameData::getInstance()->getCurPageIndex();
    if(curPageIndex >6)
    {
       curPageIndex=1;
       GameData::getInstance()->setCurPageIndex(curPageIndex);
    }
    curPageMaxLevel = GameData::getInstance()->getMaxLevel() - (curPageIndex -1) * EVERY_LEVEL_NUMES;
    if(curPageMaxLevel <=0) curPageMaxLevel=1;
    if(curPageMaxLevel >=EVERY_LEVEL_NUMES) curPageMaxLevel=EVERY_LEVEL_NUMES;
    curPlayLevel = curPageMaxLevel;
    percentPos = Vec2(0, 100);
    
    auto pos = Vec2(0.7854f*winWidth,0.0644f*winHeight);
    btnPoints.push_back(pos);
    pos = Vec2(0.8012f*winWidth,0.2006f*winHeight);
    btnPoints.push_back(pos);
    pos = Vec2(0.8773f*winWidth,0.3026f*winHeight);
    btnPoints.push_back(pos);
    pos = Vec2(0.9633f*winWidth,0.3426f*winHeight);
    btnPoints.push_back(pos);
    hasPopBtn = false;
    isBtnMoveing = false;
    popBtnNum = Pop_Btn_Num;
    
    setKeyboardEnabled(true);
}

void LevelLayer::initView()
{
    Node* root = CSLoader::createNode("LevelLayer.csb");
    root->setTag(Root_Tag);
    this->addChild(root);
    
    auto bg = (Sprite*)seekNodeByName(root, "levelBg");
    bgSize = bg->getContentSize();
    const char* bgStr = String::createWithFormat("levelBg_%d.jpg",curPageIndex-1)->getCString();
    bg->setTexture(bgStr);
    
    auto chapter = (Sprite*)seekNodeByName(root, "chapter");
    const char* chapterStr = String::createWithFormat("ui-chapter%d.png",curPageIndex)->getCString();
    chapter->setSpriteFrame(chapterStr);
    
    auto preChapBtn = (Button*)seekNodeByName(root, "preChapterBtn");
    preChapBtn->setVisible(true);
    preChapBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::preChap_callBack, this));
    if(curPageIndex == 1) preChapBtn->setVisible(false);
    
    menuBtn = (Button*)seekNodeByName(root, "menuBtn");
    menuBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::menuBtn_callBack, this));
    shopBtn = (Button*)seekNodeByName(root, "shopBtn");
    shopBtn->setVisible(false);
    shopBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::menuBtn_callBack, this));
    packBtn = (Button*)seekNodeByName(root, "packBtn");
    packBtn->setVisible(false);
    packBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::menuBtn_callBack, this));
    friendBtn = (Button*)seekNodeByName(root, "friendBtn");
    friendBtn->setVisible(false);
    friendBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::menuBtn_callBack, this));
    discountBtn = (Button*)seekNodeByName(root, "discountBtn");
    discountBtn->setVisible(false);
    discountBtn->addTouchEventListener(CC_CALLBACK_2(LevelLayer::menuBtn_callBack, this));
    
    scView = (ScrollView*)seekNodeByName(root, "scrollView");
    auto innerSize = scView->getInnerContainer()->getContentSize();
    auto scViewSize = scView->getContentSize();
    scViewPos = Vec2(innerSize.width - scViewSize.width,scViewSize.height-innerSize.height);
    auto bFN = btnFramNames[curPageIndex-1];
    for(int i=1;i<=EVERY_LEVEL_NUMES+1;i++)
    {
        const char* btName = String::createWithFormat("button_%02d",i)->getCString();
        auto node = (Button*)seekNodeByName(root, btName);
        auto pos =  convertToNodeSpace(node->getPosition());
        node->addTouchEventListener(CC_CALLBACK_2(LevelLayer::btn_callBack, this));
    
        if(i != EVERY_LEVEL_NUMES+1)
        {
            if(i <= curPageMaxLevel)
            {
                string norFile = bFN.norFile;
                string selFile = bFN.selFile;
                if(i==curPageMaxLevel)
                {
                    norFile = bFN.maxNorFile;
                    norFile = bFN.maxSelFile;
                }
                auto disNorFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(norFile);
                auto disSelFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(selFile);
                //cocosstudi让btn设置为false 再再程序中设置为true才可以替换frame
                node->setScale9Enabled(true);
                node->_buttonNormalRenderer->setSpriteFrame(disNorFrame);
                node->_buttonClickedRenderer->setSpriteFrame(disSelFrame);
            }
            else
            {
                node->setTouchEnabled(false);
                node->_titleRenderer->setVisible(false);
            }
        }
        else
        {
            auto node = seekNodeByName(root, "nextLevel");
            pos =  convertToWorldSpace(node->getPosition());
        }
       
        //log("i = [posX:%f posy:%f tag:%d]",pos.x,pos.y,node->getTag());
        levPoints.push_back(pos);
    }
    
    auto lock = seekNodeByName(root, "nextLevel");
    int maxPageIndex = GameData::getInstance()->getMaxPageIndex();
    if(maxPageIndex > curPageIndex)
    {
        lock->setVisible(false);
    }
    
    
    for(int i=1;i<=EVERY_LEVEL_NUMES;i++)
    {
        const char* dotName = String::createWithFormat("dot%d",i)->getCString();
        auto dot = (Node*)seekNodeByName(root, dotName);
        dot->setColor(bFN.feetColor);
    }
    
    
    hero = (Sprite*)seekNodeByName(root, "hero");
    float hIndex = curPageMaxLevel-1;
    auto hPos = levPoints[hIndex];
    hPos = hPos + Vec2(0, HERO_JUMP_HEIGHT);
    hero->setPosition(hPos);
    
    auto hMove = EaseIn::create(MoveBy::create(0.5f, Vec2(0,30)),0.8f);
    auto hReAct = RepeatForever::create(Sequence::create(hMove,hMove->reverse(), NULL));
    hero->runAction(hReAct);
    
    //移动到目标位置
    //toTargetLevel(curPlayLevel,curPageMaxLevel);
    //scView->scrollToBottomLeft(2.5, true);
    
    auto aimPerVec2 = percentPos - getToPerVec2(levPoints[hIndex]);
    scView->scrollToPercentBothDirection(aimPerVec2,0.5f, true);
    
    auto btnNode = (Node*)seekNodeByName(root, "buttonNode");
    curLevSp = Sprite::createWithSpriteFrameName("ui-hore.png");
    curLevSp->setPosition(levPoints[hIndex]);
    curLevSp->setScale(3.0f);
    float time1 = 0.8f;
    auto fadeIn = FadeIn::create(time1);
    auto scale1 = ScaleTo::create(time1, 3.5f);
    auto fadeOut = FadeOut::create(time1*0.5f);
    auto scale2 = ScaleTo::create(time1*0.5f, 2.8f);
    auto spawn1 = Spawn::create(EaseIn::create(fadeIn,0.5),scale1, NULL);
    auto spawn2 = Spawn::create(EaseOut::create(fadeOut,0.5f),scale2, NULL);
    auto reAction = RepeatForever::create(Sequence::create(spawn1,spawn2, NULL));
    curLevSp->runAction(reAction);
    btnNode->addChild(curLevSp);
    
    popMenuBtn();
}

void LevelLayer::preChap_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        log("preChap_callBack");
        //上一个章节
        int cpIndex = GameData::getInstance()->getCurPageIndex();
        GameData::getInstance()->setCurPageIndex(cpIndex-1);
//        GameData::getInstance()->setLevel(1);
        auto nextSc = LevelLayer::scene();
        Director::getInstance()->replaceScene(TransitionMoveRL::create(2.5f, nextSc));
    }
}

void LevelLayer::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Audio::getInstance()->playEffect(sound_click);
        log("btn_callBack");
        Button* btn = (Button*)pSender;
        int level = btn->getTag();
//        toTargetLevel(curPlayLevel,level);
//        curPlayLevel = level;
//        return;
        int trueLevel = (curPageIndex-1) * EVERY_LEVEL_NUMES + level;
        log("trueLevel:%d",trueLevel);
        GameData::getInstance()->setLevel(trueLevel);
        if(level <= EVERY_LEVEL_NUMES)
        {
            auto dialog =  AimDialog::create();
            addChild(dialog);
        }
        else
        {
            int maxPageIndex = GameData::getInstance()->getMaxPageIndex();
            if(maxPageIndex > curPageIndex)
            {
                int cpIndex = GameData::getInstance()->getCurPageIndex();
                GameData::getInstance()->setCurPageIndex(cpIndex+1);
                //GameData::getInstance()->setMaxLevel(EVERY_LEVEL_NUMES*cpIndex+1);
                auto nextSc = LevelLayer::scene();
                Director::getInstance()->replaceScene(TransitionMoveLR::create(2.5f, nextSc));
            }
            else
            {
                Toast::toast("您还没有解锁");
            }
        }
    }
}

void LevelLayer::toTargetLevel(int curPlayLevel , int tarLevel,bool isResetHeroPos)
{
    log("toTargetLevel:%d",tarLevel);
    
//    //到下一章节
//    if(tarLevel % (EVERY_LEVEL_NUMES+1) == 0)
//    {
//        log("nextChaptor");
//        int cpIndex = GameData::getInstance()->getCurPageIndex();
//        GameData::getInstance()->setCurPageIndex(cpIndex+1);
//        auto sc = LevelLayer::scene();
//        Director::getInstance()->replaceScene(TransitionMoveInR::create(2.5f, sc));
//        return;
//    }
    
    int curPosIndex = curPlayLevel-1 - (curPageIndex-1) * EVERY_LEVEL_NUMES;
    int aimPosIndex = tarLevel-1-(curPageIndex-1) * EVERY_LEVEL_NUMES;
    auto curPos = levPoints[curPosIndex];
    auto aimPos = levPoints[aimPosIndex];
    float dis = curPos.distance(aimPos);
    log("dis:%f",dis);
    
    if(isResetHeroPos) hero->setPosition(curPos + Vec2(0, HERO_JUMP_HEIGHT));
    if(dis < 10) dis = 10;
    float moveTime = dis * 0.01f;
    auto moveBy = MoveBy::create(moveTime, aimPos-curPos);
    auto callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::move_callBack,this));
    auto seq = Sequence::create(moveBy,DelayTime::create(0.2f),callBack, NULL);
    hero->runAction(seq);
    Vec2 perPos = getToPerVec2(aimPos);
    Vec2 curPrePos = percentPos - perPos;
    curPos = scView->getInnerContainer()->getPosition();
    scView->scrollToPercentBothDirection(curPrePos, moveTime, true);

}

void LevelLayer::move_callBack(cocos2d::Ref *pSender)
{
    log("move_callBack");
    Audio::getInstance()->playEffect(sound_newLevel);
    int index = curPageMaxLevel-1;
    auto pos = levPoints[index];
    if(index < EVERY_LEVEL_NUMES)
    {
        curLevSp->setPosition(pos);
        auto dialog =  AimDialog::create();
        addChild(dialog);
    }
    else
    {
        //下一个章节
        int cpIndex = GameData::getInstance()->getCurPageIndex();
        GameData::getInstance()->setCurPageIndex(cpIndex+1);

        
        vector<Vec2> posVec;
        posVec.push_back(Vec2(54.85,-67.51));
        posVec.push_back(Vec2(-222.81,43.30));
        posVec.push_back(Vec2(121.83,80.70));
        posVec.push_back(Vec2(-203.08,-161.27));
        auto root = getChildByTag(Root_Tag);
        float time = 0.8f;
        for(int i=0;i<4;i++)
        {
            const char* lockStr = String::createWithFormat("lock%d",i)->getCString();
            auto lock = seekNodeByName(root,lockStr);
            auto moveTo = MoveTo::create(time, posVec[i]);
            auto remove = RemoveSelf::create();
            auto seq = Sequence::create(EaseOut::create(moveTo, 0.1f),remove, NULL);
            lock->runAction(seq);
        }
        
        auto callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::lock_callBack,this));
        auto nextAction = Sequence::create(DelayTime::create(time),callBack, NULL);
        runAction(nextAction);
        
//        auto nextSc = LevelLayer::scene();
//        Director::getInstance()->replaceScene(TransitionMoveInR::create(2.5f, nextSc));
    }
}

Vec2 LevelLayer::getToPerVec2(Vec2 tarPos)
{
    int x = MAX(tarPos.x, winWidth / 2);
    int y = MAX(tarPos.y, winHeight/ 2);
    x = MIN(x, bgSize.width - winWidth/2);
    y = MIN(y, bgSize.height - winHeight/2);
    auto goodPoint = Vec2(x, y);
    auto centerOfScreen = winCenter;
    centerOfScreen.subtract(goodPoint);
    float perX = centerOfScreen.x/scViewPos.x*100;
    float perY = centerOfScreen.y/scViewPos.y*100;
    return Vec2(perX , perY);
}

void LevelLayer::menuBtn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        if(isBtnMoveing) return;
        Audio::getInstance()->playEffect(sound_click);
        Button* btn = (Button*)pSender;
        int tagIndex = btn->getTag() - MENU_TAG_BASE;
        switch (tagIndex)
        {
            case 0:
                log("menu_callBack");
                popMenuBtn();
                break;
            case 1:
                log("pack_callBack");
                {
                    auto layer = PackageLayer::create();
                    addChild(layer);
                }
                break;
            case 2:
                log("friend_callBack");
                {
                    GameData::getInstance()->setQuaryTotalScore(true);
                    auto rankeLayer = RankLayer::create();
                    rankeLayer->loadLeaderboard(0);
                    addChild(rankeLayer);
                }
                break;
            case 3:
                log("shop_callBack");
                {
                    auto layer = ShopLayer::create();
                    addChild(layer);
                }
                break;
            case 4:
                log("dicount_callBack");
                break;
            default:
                break;
        }
    }
}

void LevelLayer::popMenuBtn()
{
    isBtnMoveing = true;
    float moveTime = 0.3f;
    if(hasPopBtn == false)
    {
        auto curPos = menuBtn->getPosition();
        int index = packBtn->getTag() - MENU_TAG_BASE - 1;
        auto toPos = btnPoints[index];
        auto moveTo = MoveTo::create(moveTime, toPos);
        auto scaleTo = ScaleTo::create(moveTime, 1.0f);
        auto callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::btnPop_callBack,this));
        auto seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),callBack, NULL);
        packBtn->setVisible(true);
        packBtn->setPosition(curPos);
        packBtn->runAction(EaseOut::create(seq,0.5f));
        
        index = friendBtn->getTag() - MENU_TAG_BASE - 1;
        toPos = btnPoints[index];
        moveTo = MoveTo::create(moveTime, toPos);
        scaleTo = ScaleTo::create(moveTime, 1.0f);
        callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::btnPop_callBack,this));
        seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),callBack, NULL);
        friendBtn->setVisible(true);
        friendBtn->setPosition(curPos);
        friendBtn->runAction(EaseOut::create(seq,0.5f));
        
        
        index = shopBtn->getTag() - MENU_TAG_BASE - 1;
        toPos = btnPoints[index];
        moveTo = MoveTo::create(moveTime, toPos);
        scaleTo = ScaleTo::create(moveTime, 1.0f);
        callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::btnPop_callBack,this));
        seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),callBack, NULL);
        shopBtn->setVisible(true);
        shopBtn->setPosition(curPos);
        shopBtn->runAction(EaseOut::create(seq,0.5f));
        
        
        index = discountBtn->getTag() - MENU_TAG_BASE - 1;
        toPos = btnPoints[index];
        moveTo = MoveTo::create(moveTime, toPos);
        scaleTo = ScaleTo::create(moveTime, 1.0f);
        callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::btnPop_callBack,this));
        seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),callBack, NULL);
        discountBtn->setVisible(true);
        discountBtn->setPosition(curPos);
        //discountBtn->runAction(EaseOut::create(seq,0.5f));
        
        return;
    }
    
    
    auto toPos = menuBtn->getPosition();
    int index = packBtn->getTag() - MENU_TAG_BASE - 1;
    auto moveTo = MoveTo::create(moveTime, toPos);
    auto scaleTo = ScaleTo::create(moveTime, 0.2f);
    auto callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::btnPop_callBack,this));
    auto seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),callBack, NULL);
    packBtn->setVisible(true);
    packBtn->runAction(EaseIn::create(seq,0.5f));
    
    index = friendBtn->getTag() - MENU_TAG_BASE - 1;
    seq = Sequence::create(Spawn::create(moveTo->clone(),scaleTo->clone(), NULL),callBack, NULL);
    friendBtn->runAction(EaseIn::create(seq,0.5f));
    
    
    index = shopBtn->getTag() - MENU_TAG_BASE - 1;
    seq = Sequence::create(Spawn::create(moveTo->clone(),scaleTo->clone(), NULL),callBack, NULL);
    shopBtn->setVisible(true);
    shopBtn->runAction(EaseIn::create(seq,0.5f));
    
    
    index = discountBtn->getTag() - MENU_TAG_BASE - 1;
    seq = Sequence::create(Spawn::create(moveTo->clone(),scaleTo->clone(), NULL),callBack, NULL);
    discountBtn->setVisible(true);
    //discountBtn->runAction(EaseIn::create(seq,0.5f));
    
}

void LevelLayer::btnPop_callBack(Node* pSender)
{
    pSender->setVisible(!hasPopBtn);
    popBtnNum--;
    if(popBtnNum == 0)
    {
        isBtnMoveing = false;
        hasPopBtn = !hasPopBtn;
        popBtnNum = Pop_Btn_Num;
    }
}

void LevelLayer::lock_callBack(cocos2d::Node *pSender)
{
    log("lock_callBack");
    
    //GameData::getInstance()->setMaxLevel(EVERY_LEVEL_NUMES*cpIndex+1);
    auto nextSc = LevelLayer::scene();
    Director::getInstance()->replaceScene(TransitionMoveLR::create(2.5f, nextSc));
}

void LevelLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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
