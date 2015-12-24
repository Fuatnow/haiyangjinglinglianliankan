//
//  MenuLayer.cpp
//  Link
//
//  Created by Fuatnow on 15-3-22.
//
//

#include "MenuLayer.h"
#include "Tools.h"
#include "GameLayer.h"
#include "LevelManager.h"
#include "Audio.h"
#include "Toast.h"
#include "Platform.h"
MenuLayer* MenuLayer::create(GameLayer *game)
{
    MenuLayer* layer = new MenuLayer;
    layer->_game = game;
    layer->init();
    layer->autorelease();
    return layer;
}

bool MenuLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void MenuLayer::initData()
{
    board = _game->board;
    MapInfo mapInfo = GameData::getInstance()->getMapInfo();
    levelNum = GameData::getInstance()->getLevel();
    shortTime = mapInfo.aimpassInfo.shortTime;
    lenTime = mapInfo.aimpassInfo.lenTime;
    shortTimer = 0;
    lenTimer = 0;
    isSerialLink = false;
    timeNotEnough = false;
    
    
    searchNum = GameData::getInstance()->getTakeSearchNum();
    swapNum = GameData::getInstance()->getTakeSwapNum();
    bombNum = GameData::getInstance()->getTakeBombNum();
    
    
    leftNum = board->getHasCellCount();
    scoreNum = 0;
    serialLinkTimes = 0;
    leftTime = 0;
    createCellTimer = 0;
    int increateNum = mapInfo.increaseData;
    createCellInterval = increateNum / 100;
    createCellParis = (increateNum / 10) % 10;
    if(increateNum < 100) createCellInterval = 9999999;
}

void MenuLayer::initView()
{
    Node* root = CSLoader::createNode("MenuLayer.csb");
    this->addChild(root);
    
    Button* swapItem = (Button*)seekNodeByName(root, "swapItem");
    swapItem->addTouchEventListener(CC_CALLBACK_2(MenuLayer::swap_callBack, this));
    Button* searchItem = (Button*)seekNodeByName(root, "searchItem");
    searchItem->addTouchEventListener(CC_CALLBACK_2(MenuLayer::search_callBack, this));
    Button* bombItem = (Button*)seekNodeByName(root, "bombItem");
    bombItem->addTouchEventListener(CC_CALLBACK_2(MenuLayer::bomb_callBack, this));
    Button* menuItem = (Button*)seekNodeByName(root, "menuItem");
    menuItem->addTouchEventListener(CC_CALLBACK_2(MenuLayer::menu_callBack, this));
    
    std::stringstream ss;
    ss << lenTime;
    alarmNumLab = (TextBMFont*)seekNodeByName(root, "timeLabel");
    alarmNumLab->setString(ss.str());
    ss.clear();
    ss.str("");
    ss << leftNum;
    leftLabel = (TextBMFont*)seekNodeByName(root, "leftLabel");
    leftLabel->setString(ss.str());
    ss.clear();
    ss.str("");
    ss << levelNum;
    levelLabel = (TextBMFont*)seekNodeByName(root, "levelLabel");
    levelLabel->setString(ss.str());
    ss.clear();
    ss.str("");
    ss << scoreNum;
    scoreLabel = (TextBMFont*)seekNodeByName(root, "scoreLabel");
    scoreLabel->setString(ss.str());
    
    int bestScore = GameData::getInstance()->getBestScoreNum(levelNum);
    ss.clear();
    ss.str("");
    ss << bestScore;
    bestScoreLabel = (TextBMFont*)seekNodeByName(root, "bestScoreLabel");
    bestScoreLabel->setString(ss.str());
    ss.clear();
    ss.str("");
    ss << swapNum ;
    swapNumLab = (TextBMFont*)seekNodeByName(root, "swapNumLab");
    swapNumLab->setString("x"+ss.str());
    ss.clear();
    ss.str("");
    ss << searchNum;
    searchNumLab = (TextBMFont*)seekNodeByName(root, "searchNumLab");
    searchNumLab->setString("x"+ss.str());
    ss.clear();
    ss.str("");
    ss << bombNum;
    bombNumLab = (TextBMFont*)seekNodeByName(root, "bombNumLab");
    bombNumLab->setString("x"+ss.str());
    
    lenBar = (LoadingBar*)seekNodeByName(root, "lenBar");
    shortBar = (LoadingBar*)seekNodeByName(root, "shortBar");
    
    
    scheduleUpdate();
}

void MenuLayer::swap_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        if(swapNum <=0)
        {
            Toast::toast("该局[交换]道具已经使用完毕");
        }
        
        log("swap_callBack");
        if(board->isMoving == false && board->isReady == true
           && swapNum > 0 && _game->getGameState() == Game_Play)
        {
            swapNum -= 1;
            std::stringstream ss;
            ss << swapNum;
            swapNumLab->setString("x"+ss.str());
            board->exchangeCells(true);
            Audio::getInstance()->playEffect(sound_swap);
            
            int totalSwapNum = GameData::getInstance()->getSwapNum() - 1;
            GameData::getInstance()->setSwapNum(totalSwapNum);
            
            // 使用道具.
            umeng::MobClickCpp::use("swapProp", 1 , 1);
        }
    }
}

void MenuLayer::search_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        log("search_callBack");
        
        if(searchNum <=0)
        {
            Toast::toast("该局[自动消除]道具已经使用完毕");
        }
        
        if(board->isMoving == false && board->isReady == true
           && searchNum > 0 && _game->getGameState() == Game_Play)
        {
            searchNum -= 1;
            std::stringstream ss;
            ss << searchNum;
            searchNumLab->setString("x"+ss.str());
            board->removeOnePairCell();
            Audio::getInstance()->playEffect(sound_search);
            
            
            int totalSearchNum = GameData::getInstance()->getSearchNum() - 1;
            GameData::getInstance()->setSearchNum(totalSearchNum);
            
            // 使用道具.
            umeng::MobClickCpp::use("searchProp", 1 , 1);
        }
    }
}

void MenuLayer::bomb_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        log("bomb_callBack");
        
        if(bombNum <=0)
        {
            Toast::toast("该局[炸弹]道具已经使用完毕");
        }
        
        if(board->isMoving == false && board->isReady == true
           && bombNum>0 && _game->getGameState() == Game_Play)
        {
            bombNum -= 1;
            std::stringstream ss;
            ss << bombNum;
            bombNumLab->setString("x"+ss.str());
            board->removeOneType();
            Audio::getInstance()->playEffect(sound_bomb);
            
            int totalBombNum = GameData::getInstance()->getBombNum() - 1;
            GameData::getInstance()->setBombNum(totalBombNum);
            
            // 使用道具.
            umeng::MobClickCpp::use("bombProp", 1 , 1);
        }
    }
}

void MenuLayer::menu_callBack(Ref *pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
        {
            log("BEGAN");
            //board->fallToBoard();
        }
            break;
        case Widget::TouchEventType::MOVED:
        {
            log("MOVED");
        }
            break;
        case Widget::TouchEventType::ENDED:
        {
            if(board->isMoving == false && board->isReady == true
               && _game->getGameState() == Game_Play)
            {
                _game->pauseGame();
            }
            log("ENDED");
        }
            break;
        case Widget::TouchEventType::CANCELED:
        {
            log("CANCELED");
        }
            break;
        default:
            break;
    }
}

void MenuLayer::update(float dt)
{
    if(board->isReady == false) return;
    
    lenTimer += dt;
    float per = 1-lenTimer/lenTime;
    if(per < 0.001f)
    {
        _game->setGameState(Game_Lose);
        _game->gameOver();
        setDisable();
        return;
    }
    
    leftTime = lenTime - lenTimer;
    //小于5秒 闹钟响起
    if(leftTime < 5.0f && timeNotEnough == false)
    {
        timeNotEnough = true;
        Audio::getInstance()->playEffect(sound_alarm);
    }
    else if(leftTime > 5.0f && timeNotEnough)
    {
        timeNotEnough = false;
        Audio::getInstance()->stopAllEffects();
    }
    
    if(leftTime+0.1f > (int)leftTime)
    {
        const char* str = String::createWithFormat("%d",(int)(leftTime+0.9f))->getCString();
        alarmNumLab->setString(str);
    }
    
    lenBar->setPercent(per*100);
    
    //连击了
    if(isSerialLink)
    {
        shortTimer += dt;
        per = 1-shortTimer/shortTime;
        if(per < 0.001f)
        {
            shortTimer = 0;
            serialLinkTimes = 0;
            isSerialLink = false;
        }
        shortBar->setPercent(per*100);
    }
    
    createNewCells(dt);
}

void MenuLayer::createNewCells(float dt)
{
    createCellTimer += dt;
    if(createCellTimer > createCellInterval)
    {
        createCellTimer = 0;
        MapInfo mapInfo = GameData::getInstance()->getMapInfo();
        board->createNextCells(createCellParis,mapInfo.kinds);
        updateLeftCellsCount(board->getHasCellCount());
    }
}


void MenuLayer::updateScore(int addScore)
{
    scoreNum += addScore;
    std::stringstream ss;
    ss << scoreNum ;
    scoreLabel->setString(ss.str());
}

void MenuLayer::updateLeftCellsCount(int hasCellCount)
{
    leftNum = hasCellCount;
    std::stringstream ss;
    ss << hasCellCount ;
    leftLabel->setString(ss.str());
}

void MenuLayer::setSerialLink(Vec2 pos)
{
    isSerialLink = true;
    shortTimer = 0;
    if(serialLinkTimes > 0)
    {
        std::stringstream ss;
        std::stringstream sc;
        ss << serialLinkTimes;
        int addScore = serialLinkTimes*2+1;
        updateScore(addScore);
        sc << addScore;
        string str = ss.str()+"连击+" + sc.str() +"分";
        auto label = Text::create(str, "normal.TTF",30);
        label->setColor(Color3B(230,238,29));
        label->setPosition(pos);
        auto moveBy = MoveBy::create(1.5f, Vec2(0,200));
        auto removeSelf = RemoveSelf::create();
        label->runAction(Sequence::create(moveBy,removeSelf, NULL));
        addChild(label,100);
        
        Audio::getInstance()->playEffect(sound_drop);
    }
    serialLinkTimes ++;
}

void MenuLayer::setDisable()
{
    unscheduleUpdate();
    setVisible(false);
}