//
//  MenuLayer.h
//  Link
//
//  Created by Fuatnow on 15-3-22.
//
//

#ifndef __Link__MenuLayer__
#define __Link__MenuLayer__

#include "BasicLayer.h"
#include "GameData.h"
#include "Board.h"
#include "LevelManager.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;

class GameLayer;

class MenuLayer : public BasicLayer
{
public:
    MenuLayer(){}
    ~MenuLayer(){}
    virtual bool init();
    void initData();
    void initView();
    void swap_callBack(Ref *pSender, Widget::TouchEventType type);
    void search_callBack(Ref *pSender, Widget::TouchEventType type);
    void bomb_callBack(Ref *pSender, Widget::TouchEventType type);
    void menu_callBack(Ref *pSender, Widget::TouchEventType type);
    void updateScore(int addScore);
    void updateLeftCellsCount(int hasCellCount);
    virtual void update(float dt);
    void setSerialLink(Vec2 pos);
    static MenuLayer* create(GameLayer* game);
public:
    CC_SYNTHESIZE(TextBMFont*, searchNumLab, SearchNumLab);
    CC_SYNTHESIZE(TextBMFont*, swapNumLab, SwapNumLab);
    CC_SYNTHESIZE(TextBMFont*, bombNumLab, BombNumLab);
    CC_SYNTHESIZE(TextBMFont*, leftLabel, LeftLabel);
    CC_SYNTHESIZE(TextBMFont*, levelLabel, LevelLabel);
    CC_SYNTHESIZE(TextBMFont*, scoreLabel, ScoreLabel);
    CC_SYNTHESIZE(TextBMFont*, bestScoreLabel, BestScoreLabel);
    CC_SYNTHESIZE(LoadingBar*, lenBar, LenBar);
    CC_SYNTHESIZE(LoadingBar*, shortBar, ShortBar);
private:
    int levelNum;
    int searchNum;
    int swapNum;
    int bombNum;
    int leftNum;
    int scoreNum;
    float shortTime;
    float shortTimer;
    float lenTime;
    float lenTimer;
    bool isSerialLink;//是否连击
    int serialLinkTimes;//连击次数
    GameLayer* _game;
    Board* board ;
};

#endif /* defined(__Link__MenuLayer__) */
