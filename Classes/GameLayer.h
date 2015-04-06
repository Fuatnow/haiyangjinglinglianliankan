#ifndef	__GameLayer_H_
#define __GameLayer_H_
#include "BasicLayer.h"
#include "LevelManager.h"
#include "BackgroundLayer.h"
#include "MenuLayer.h"

enum GameState
{
    Game_Play,
    Game_Menu,
    Game_Pause,
    Game_Lose,
    Game_Success,
};

class GameLayer : public BasicLayer
{
public:
	GameLayer(){}
	~GameLayer(){}
	static Scene* scene();
	virtual bool init();
	void initData();
	void initView();
    void gameOver();
    void pauseGame();
    void resumeGame();
	CREATE_FUNC(GameLayer);
public:
	LevelManager* levelManager;
    BackgroundLayer* backgroundLayer;
    MenuLayer* menuLayer;
    Board* board;;
    CC_SYNTHESIZE(GameState, gameState, GameState)
};
#endif