#include "GameData.h"
#include "Platform.h"
static GameData *gamedata = NULL;
GameData::GameData()
{
    
}

GameData::~GameData()
{
    log("~GameData");
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
    appVersion = 1;
    maxLevel = userSave->getIntegerForKey("maxLevel",1);
//    maxLevel = 150;
    curlevel = maxLevel;
    swapNum = userSave->getIntegerForKey("swapNum",4);
    searchNum = userSave->getIntegerForKey("searchNum",4);
    bombNum = userSave->getIntegerForKey("bombNum",3);
    timeNum = userSave->getIntegerForKey("timeNum",3);
    takebombNum = 0;
    takeSearchNum = 0;
    takeSwapNum = 0;
    takeTimeNum = 0;
    maxTakeTimeNum = 1;
    maxTakebombNum = 2;
    maxTakeSearchNum = 3;
    maxTakeSwapNum = 3;
    curPageIndex = 1 + (maxLevel-1)/EVERY_LEVEL_NUMES;
    curLevelStar = 0;
    curLevelScore = 0;
    coinNum = userSave->getIntegerForKey("coinNum",108);
    propKindsNum = 4;
    diamondNum = userSave->getIntegerForKey("diamondNum",10);
    maxPageIndex = userSave->getIntegerForKey("maxPageIndex",1);
    //是否玩过游戏
    hadOncePlayGame = userSave->getBoolForKey("hadOncePlayGame",false);
    hadGiveUpdateAward = userSave->getBoolForKey("hadGiveUpdateAward",false);
    preClickReputationLevel = userSave->getIntegerForKey("preClickReputationLevel",1);
    quaryTotalScore = false;
    //是否需要强制更新游戏
    haveNeedForeUpdateAPP = false;
    log("---------->%s",userSave->getXMLFilePath().c_str());
    initAppChanel();
}

void GameData::initAppChanel()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value pay(rapidjson::kObjectType);
    rapidjson::Value cmd(rapidjson::kObjectType);
    rapidjson::Value info(rapidjson::kArrayType);
    int appVersion = GameData::getInstance()->getAppVersion();
    const char* verStr = String::createWithFormat("%d",appVersion)->getCString();
    
    cmd.AddMember("cmdId", cmd_getChannel, allocator);
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
    GameData::getInstance()->setAppChanel(rtnStr);
}


void GameData::initObjectPool()
{
    //初始化101X2个对象
    for(int i=0;i<101;i++)
    {
        auto rankNode1 =  CSLoader::createNode("RankItem.csb");
        rankNode1->retain();
        globalRankItems.push_back(rankNode1);
        auto rankNode2 =  CSLoader::createNode("RankItem.csb");
        rankNode2->retain();
        friendsRankItems.push_back(rankNode2);
    }
}

int GameData::getBestScoreNum(int level)
{
    if(level == 0) level = curlevel;
    const char* levelStr = String::createWithFormat("level%d",level)->getCString();
    int bestScore = userSave->getIntegerForKey(levelStr,0);
    return bestScore;
}

void GameData::setMaxLevel(int level)
{
    if(maxLevel < level)
    {
        maxLevel = level;
        userSave->setIntegerForKey("maxLevel", maxLevel);
    }
}

int GameData::getMaxLevel()
{
    return maxLevel;
}

void GameData::setCurLevelScore(int score)
{
    const char* levelStr = String::createWithFormat("level%d",curlevel)->getCString();
    curLevelScore = score;
    int maxCurLebelScore = userSave->getIntegerForKey(levelStr,0);
    if(maxCurLebelScore < curLevelScore) userSave->setIntegerForKey(levelStr, curLevelScore);
}


int GameData::getCurLevelScore()
{
    return curLevelScore;
}


void GameData::setMaxPageIndex(int var)
{
    int max = userSave->getIntegerForKey("maxPageIndex",1);
    if(var > max)
    {
        userSave->setIntegerForKey("maxPageIndex", var);
        maxPageIndex = var;
    }
}

int GameData::getMaxPageIndex()
{
    return maxPageIndex;
}

void GameData::setCurPageIndex(int var)
{
    if(var > maxPageIndex) setMaxPageIndex(var);
    curPageIndex = var;
}

int GameData::getCurPageIndex()
{
    return curPageIndex;
}

void GameData::setDiamondNum(int var)
{
    userSave->setIntegerForKey("diamondNum", var);
    diamondNum = var;
}

int GameData::getDiamondNum()
{
    return diamondNum;
}

void GameData::setSearchNum(int var)
{
    userSave->setIntegerForKey("searchNum", var);
    searchNum = var;
}

int GameData::getSearchNum()
{
    return searchNum;
}

void GameData::setSwapNum(int var)
{
    userSave->setIntegerForKey("swapNum", var);
    swapNum = var;
}

int GameData::getSwapNum()
{
    return swapNum;
}

void GameData::setBombNum(int var)
{
    userSave->setIntegerForKey("bombNum", var);
    bombNum = var;
}

int GameData::getBombNum()
{
    return bombNum;
}

void GameData::setTimeNum(int var)
{
    userSave->setIntegerForKey("timeNum", var);
    timeNum = var;
}

int GameData::getTimeNum()
{
    return timeNum;
}

void GameData::setCoinNum(int var)
{
    userSave->setIntegerForKey("coinNum", var);
    coinNum = var;
}

int GameData::getCoinNum()
{
    return coinNum;
}

void GameData::setCurLevelStar(int var)
{
    userSave->setIntegerForKey("curLevelStar", var);
    curLevelStar = var;
}

int GameData::getCurLevelStar()
{
    return curLevelStar;
}

void GameData::setHadOncePlayGame(bool var)
{
    userSave->setBoolForKey("hadOncePlayGame", var);
    hadOncePlayGame = var;
}

bool GameData::getHadOncePlayGame()
{
    return hadOncePlayGame;
}

void GameData::setHadGiveUpdateAward(bool var)
{
    userSave->setBoolForKey("hadGiveUpdateAward", var);
    hadGiveUpdateAward = var;
}

bool GameData::getHadGiveUpdateAward()
{
    return hadGiveUpdateAward;
}

void GameData::setTakeSearchNum(int var)
{
    takeSearchNum = var;
}

int GameData::getTakeSearchNum()
{
    int searchNum = GameData::getInstance()->getSearchNum();
    int maxTakeSearchNum = GameData::getInstance()->getMaxTakeSearchNum();
    takeSearchNum = searchNum > maxTakeSearchNum ? maxTakeSearchNum : searchNum;
    return takeSearchNum;
}

void GameData::setTakeSwapNum(int var)
{
    takeSwapNum = var;
}

int GameData::getTakeSwapNum()
{
    int swapNum = GameData::getInstance()->getSwapNum();
    int maxTakeSwapNum = GameData::getInstance()->getMaxTakeSwapNum();
    takeSwapNum = swapNum > maxTakeSwapNum ? maxTakeSwapNum : swapNum;
    return takeSwapNum;
}

void GameData::setTakeBombNum(int var)
{
    takebombNum = var;
}

int GameData::getTakeBombNum()
{
    int bombNum = GameData::getInstance()->getBombNum();
    int maxTakebombNum = GameData::getInstance()->getMaxTakeBombNum();
    takebombNum = bombNum > maxTakebombNum ? maxTakebombNum : bombNum;
    return takebombNum;
}

void GameData::setTakeTimeNum(int var)
{
    takeTimeNum = var;
}

int GameData::getTakeTimeNum()
{
    int timeNum = GameData::getInstance()->getTimeNum();
    int maxTakeTimeNum = GameData::getInstance()->getMaxTakeTimeNum();
    takeSwapNum = timeNum > maxTakeTimeNum ? maxTakeTimeNum : timeNum;
    return takeTimeNum;
}

void GameData::setPreClickReputationLevel(int var)
{
    userSave->setIntegerForKey("preClickReputationLevel", var);
    preClickReputationLevel = var;
}

int GameData::getPreClickReputationLevel()
{
    return preClickReputationLevel;
}

void GameData::addCoin(int coin)
{
    coinNum += coin;
    userSave->setIntegerForKey("coinNum", coinNum);
}

void GameData::addDiamond(int diamond)
{
	diamondNum += diamond;
	userSave->setIntegerForKey("diamondNum", diamondNum);
}

vector<int>& GameData::getPropNums()
{
    propkindNumsVec.clear();
    propkindNumsVec.push_back(swapNum);
    propkindNumsVec.push_back(searchNum);
    propkindNumsVec.push_back(bombNum);
    propkindNumsVec.push_back(timeNum);
    return propkindNumsVec;
}

vector<Node*>& GameData::getGlobalRankItems()
{
    return globalRankItems;
}

vector<Node*>& GameData::getFriendsRankItems()
{
    return friendsRankItems;
}

int GameData::getTotalScore()
{
    int totalScore = 0;
    for(int i=1;i<=maxLevel;i++)
    {
        const char* levelStr = String::createWithFormat("level%d",i)->getCString();
        int levelScore = userSave->getIntegerForKey(levelStr,0);
        //log("level:%d Score:%d",i,levelScore);
        totalScore += levelScore;
    }
    return totalScore;
}
