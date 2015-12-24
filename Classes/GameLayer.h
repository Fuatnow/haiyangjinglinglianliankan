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
    void initShareBtn(Node* par,bool canReputation);
    void gameOver();
    void pauseGame();
    void resumeGame();
    void reportTotalScore();
    void loadAD();
    void getReputation();
    void registerCallBack(Ref *pSender, Widget::TouchEventType type);
    void shareCallBack(Ref *pSender, Widget::TouchEventType type);
    void feedBackCallBack(Ref *pSender, Widget::TouchEventType type);
    void reputationCallBack(Ref *pSender, Widget::TouchEventType type);
    void capture(bool succeed, const std::string& outputFile);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	CREATE_FUNC(GameLayer);
public:
	LevelManager* levelManager;
    BackgroundLayer* backgroundLayer;
    MenuLayer* menuLayer;
    Board* board;
    CC_SYNTHESIZE(GameState, gameState, GameState)
};
#endif