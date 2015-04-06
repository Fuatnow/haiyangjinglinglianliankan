#include "GameLayer.h"
#include "SuccessDialog.h"
#include "PauseDialog.h"
#include "LoseDialog.h"
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
    levelManager->loadLevel(level);
	levelManager = LevelManager::create();
    
    MapInfo info = GameData::getInstance()->getMapInfo();
    levelManager->gainMapInfo(info,level);
    GameData::getInstance()->setMapInfo(info);
	addChild(levelManager);
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

void GameLayer::gameOver()
{
    menuLayer->setVisible(false);
    if(gameState == Game_Success)
    {
        CCLOG("game_win");
        auto dialog =  SuccessDialog::create();
        addChild(dialog,100);
    }
    else if(gameState == Game_Lose)
    {
        CCLOG("game_lose");
//        auto dialog =  LoseDialog::create();
//        addChild(dialog,100);
    }
}

void GameLayer::pauseGame()
{
    gameState = Game_Pause;
    auto pauseDialog = PauseDialog::create();
    addChild(pauseDialog,100);
    this->pause();
    pauseDialog->resume();
}


void GameLayer::resumeGame()
{
    gameState = Game_Play;
    Director::getInstance()->resume();
}
