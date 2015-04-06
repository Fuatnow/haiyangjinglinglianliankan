#ifndef LevelManager_H__
#define LevelManager_H__
#include "BasicLayer.h"
#include "GameData.h"
#include "Board.h"
#include "Define.h"
#include "MapInfo.h"
using namespace std;

class LevelManager : public BasicLayer
{
public:
	LevelManager(){};
	~LevelManager(){};
	virtual bool init();
	void initData();
	void initView();
	void loadLevel(int level);
    void gainMapInfo(MapInfo &mapInfo , int level);
	void debugMap(int row,int col);
    void split(std::vector<std::string> &result,std::string str,std::string pattern);
	CREATE_FUNC(LevelManager);
public:
	vector<int>mapVec[ROW+2];
};
#endif