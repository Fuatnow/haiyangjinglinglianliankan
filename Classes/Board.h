#ifndef Board_H__
#define Board_H__
#include "BasicLayer.h"
#include "Cell.h"
#include "Define.h"
using namespace std;

struct TurnStepPath
{
    //步数路径
    vector<Vec3> setpVec;
    //拐点
    vector<Vec3> turnVec;
};


struct Search2Pos
{
    int selRow,selCol;
    int preRow,preCol;
    bool isFind;
};

class Board : public Node
{
public:
    Board(){};
    ~Board();
    virtual bool init();
    void initData();
    void initVis();
    void addListener();
    void loadCell();
    void fallToBoard();
    bool isLegal(int row, int col);
    bool testLink(Cell* selCell, Cell* preCell);
    void removeLink(Cell* selCell, Cell* preCell);
    void link_logic(Cell* start, Cell* end);
    void playParticle(Ref* preCell);
    void playParticle_callBack(Ref* preCell);
    void removeCell_callBack(Ref* preCell);
    void delay_callBack(Ref* pSender);
    //获取拐弯数目最小且步数最少的路径
    TurnStepPath getMinTurnAndStepPath();
    void resetData();
    void debugPath(TurnStepPath pathVec);
    void debugTurn(TurnStepPath pathVec);
    void effectLink(Cell* cell,Cell* preSelCell,TurnStepPath pathVec);
    //找到一对可以消除的Cell Cell->preturn
    Search2Pos findPairCell();
    //重新排列cells
    void exchangeCells(bool isPlayAction = false);
    void exchangeCallBack();
    void debugCellVec();
    void addPath(Cell* start);
    //消除一种类型
    void removeOneType(int type = -1);
    //
    void removeOneType_callBack(Ref* preCell);
    //消除一对
    void removeOnePairCell();
    //
    void reduceCellCount(int subNum , Vec2 cellPos = Vec2(0,0));
    void updateReady(float dt);
    void debugTestData();
    void fall_callBack(Node* pSender);
    Vec2 getPositionByRowCol(int row,int col);
    CREATE_FUNC(Board);
public:
    vector<Cell*>cellVec[ROW + 2];
    vector<bool>vis[ROW + 2];
    TurnStepPath pathVec;
    vector<TurnStepPath> allPaths;
    float cellWidth;
    float cellHeight;
    int maxRow;
    int maxCol;
    int cellKinds;
    float space;
    Cell* preSelCell;
    int step_num;
    int turn_num;
    bool is_link;
    //移动是否结束
    bool isMoving;
    //是否准备好
    bool isReady;
    Vec2 centerPos;
    int countdown;
    Search2Pos sea2Pos;
    CC_SYNTHESIZE(int, hasCellCount, HasCellCount);
};
#endif