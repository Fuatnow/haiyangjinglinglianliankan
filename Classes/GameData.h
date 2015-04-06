#ifndef GameData_H_
#define GameData_H_
#include "BasicLayer.h"
#include "Define.h"
#include "MapInfo.h"
class GameData
{
public:
	GameData();
	~GameData();
    void initData();
    int getBestScoreNum(int level);
	static GameData * getInstance();
private:
    CC_PROPERTY(int, maxLevel, MaxLevel);
    CC_SYNTHESIZE(int, curlevel, Level);
    CC_SYNTHESIZE(int, searchNum, SearchNum);
    CC_SYNTHESIZE(int, swapNum, SwapNum);
    CC_SYNTHESIZE(int, bombNum, BombNum);
    CC_SYNTHESIZE(int, curPageIndex, CurPageIndex);
    CC_SYNTHESIZE(MapInfo, mapInfo, MapInfo);
};
#endif