#ifndef GameData_H_
#define GameData_H_
#include "BasicLayer.h"
#include "Define.h"
#include "MapInfo.h"
#include "RankLayer.h"
class GameData
{
public:
	GameData();
	~GameData();
    void initData();
    void initObjectPool();
    static GameData * getInstance();
    int getBestScoreNum(int level =0);
    void addCoin(int coin);
    void addDiamond(int diamond);
    void initAppChanel();
    int getTotalScore();
    vector<int>& getPropNums();
    vector<Node*>& getGlobalRankItems();
    vector<Node*>& getFriendsRankItems();
public:
    CC_PROPERTY(int, maxLevel, MaxLevel);
    CC_PROPERTY(int, curLevelStar, CurLevelStar);
    CC_PROPERTY(int, curLevelScore, CurLevelScore);
    CC_PROPERTY(int, maxPageIndex, MaxPageIndex);
    CC_PROPERTY(int, curPageIndex, CurPageIndex);
    CC_SYNTHESIZE(int, curlevel, Level);
    CC_PROPERTY(int, searchNum, SearchNum);
    CC_PROPERTY(int, swapNum, SwapNum);
    CC_PROPERTY(int, bombNum, BombNum);
    CC_PROPERTY(int, timeNum, TimeNum);
    CC_PROPERTY(int, takeSearchNum, TakeSearchNum);
    CC_PROPERTY(int, takeSwapNum, TakeSwapNum);
    CC_PROPERTY(int, takebombNum, TakeBombNum);
    CC_PROPERTY(int, takeTimeNum, TakeTimeNum);
    CC_SYNTHESIZE(int, maxTakeSearchNum, MaxTakeSearchNum);
    CC_SYNTHESIZE(int, maxTakeSwapNum, MaxTakeSwapNum);
    CC_SYNTHESIZE(int, maxTakebombNum, MaxTakeBombNum);
    CC_SYNTHESIZE(int, maxTakeTimeNum, MaxTakeTimeNum);
    CC_SYNTHESIZE(bool, quaryTotalScore, QuaryTotalScore);
    CC_SYNTHESIZE(int, appVersion, AppVersion);
    CC_SYNTHESIZE(string, appChannel, AppChanel);
    CC_SYNTHESIZE(MapInfo, mapInfo, MapInfo);
    CC_PROPERTY(int, coinNum, CoinNum);
    CC_PROPERTY(int, diamondNum, DiamondNum);
    CC_PROPERTY(bool, hadOncePlayGame, HadOncePlayGame);
    CC_SYNTHESIZE(bool, haveNeedForeUpdateAPP, HaveNeedForeUpdateAPP);
    CC_SYNTHESIZE(int, propKindsNum, PropKindsNum);
    CC_PROPERTY(bool, hadGiveUpdateAward, HadGiveUpdateAward);
    CC_PROPERTY(int , preClickReputationLevel , PreClickReputationLevel);
private:
    vector<int> propkindNumsVec;
    vector<Node*> globalRankItems;
    vector<Node*> friendsRankItems;
};
#endif
