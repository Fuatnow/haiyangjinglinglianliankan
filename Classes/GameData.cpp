#include "GameData.h"
static GameData *gamedata = NULL;
GameData::GameData()
{
    
}

GameData::~GameData()
{
    CCLOG("~GameData");
}

GameData * GameData::getInstance()
{
    if (!gamedata)
    {
        gamedata = new GameData();
        gamedata->initData();
    }
    return gamedata;
}

void GameData::initData()
{
    maxLevel = 25;
    curlevel = maxLevel;
    swapNum = 130;
    searchNum = 130;
    bombNum = 130;
    curPageIndex = 1;
}

int GameData::getBestScoreNum(int level)
{
    return level;
}

void GameData::setMaxLevel(int level)
{
    maxLevel = level;
    userSave->setIntegerForKey("maxLevel", maxLevel);
}

int GameData::getMaxLevel()
{
    return maxLevel;
}