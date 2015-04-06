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
    int curLevel = GameData::getInstance()->getLevel();
    levelNum = GameData::getInstance()->getBestScoreNum(curLevel);
    shortTime = mapInfo.aimpassInfo.shortTime;
    lenTime = mapInfo.aimpassInfo.lenTime;
    shortTimer = 0;
    lenTimer = 0;
    isSerialLink = false;
    searchNum = GameData::getInstance()->getSearchNum();
    swapNum = GameData::getInstance()->getSwapNum();
    bombNum = GameData::getInstance()->getBombNum();
    leftNum = board->getHasCellCount();
    scoreNum = 0;
    serialLinkTimes = 0;
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
        CCLOG("swap_callBack");
        if(board->isMoving == false && board->isReady == true
           && swapNum > 0 && _game->getGameState() == Game_Play)
        {
            swapNum -= 1;
            std::stringstream ss;
            ss << swapNum;
            swapNumLab->setString("x"+ss.str());
            board->exchangeCells(true);
            Audio::getInstance()->playEffect(sound_swap);
        }
    }
}

void MenuLayer::search_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        CCLOG("search_callBack");
        if(board->isMoving == false && board->isReady == true
           && searchNum > 0 && _game->getGameState() == Game_Play)
        {
            searchNum -= 1;
            std::stringstream ss;
            ss << searchNum;
            searchNumLab->setString("x"+ss.str());
            board->removeOnePairCell();
            Audio::getInstance()->playEffect(sound_search);
        }
    }
}

void MenuLayer::bomb_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        CCLOG("bomb_callBack");
        if(board->isMoving == false && board->isReady == true
           && bombNum>0 && _game->getGameState() == Game_Play)
        {
            bombNum -= 1;
            std::stringstream ss;
            ss << bombNum;
            bombNumLab->setString("x"+ss.str());
            board->removeOneType();
        }
    }
}

void MenuLayer::menu_callBack(Ref *pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
        {
            CCLOG("BEGAN");
            //board->fallToBoard();
        }
            break;
        case Widget::TouchEventType::MOVED:
        {
            CCLOG("MOVED");
        }
            break;
        case Widget::TouchEventType::ENDED:
        {
            if(board->isMoving == false && board->isReady == true
               && bombNum>0 && _game->getGameState() == Game_Play)
            {
                _game->pauseGame();
            }
            CCLOG("ENDED");
        }
            break;
        case Widget::TouchEventType::CANCELED:
        {
            CCLOG("CANCELED");
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
//        _game->setGameState(Game_Lose);
//        _game->gameOver();
//        unscheduleUpdate();
        return;
    }
    lenBar->setPercent(per*100);
    
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
        ss << serialLinkTimes ;
        string str = "连击+" + ss.str();
        auto label = Text::create(str, "normal.TTF",30);
        label->setColor(Color3B(230,238,29));
        label->setPosition(pos);
        auto moveBy = MoveBy::create(1.5f, Vec2(0,200));
        auto removeSelf = RemoveSelf::create();
        label->runAction(Sequence::create(moveBy,removeSelf, NULL));
        addChild(label,100);
    }
    serialLinkTimes ++;
}