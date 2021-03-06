#include "Board.h"
#include "math.h"
#include "GameLayer.h"
#include "MenuLayer.h"
#include "Audio.h"
#include "GameData.h"
#include "Toast.h"
#include <string>
int	dir[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };  //遍历上下左右方向（+-）1

Board::~Board()
{
    
}


bool Board::init()
{
    Node::init();
    initData();
    addListener();
    loadCell();
    return true;
}

void Board::initData()
{
    //设置种子
    timeval psv;
    gettimeofday(&psv, NULL);
    unsigned long int seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
    srand(seed);
    
    cellWidth = 72.0f;
    cellHeight = 72.0f;
    step_num = 0;
    turn_num = 0;
    space = 2;
    hasCellCount = 0;
    is_link = false;
    maxRow = ROW;
    maxCol = ROW;
    preSelCell = NULL;
    isMoving = false;
    isReady = false;
    cellKinds = 0;
    countdown = 4;
    isVecUpdatIng = false;
}

void Board::initVis()
{
    for (int row = 0; row < maxRow + 2; row++)
    {
        //        string str = "";
        for (int col = 0; col < maxCol + 2; col++)
        {
            auto cell = cellVec[row][col];
            bool isHaveCell = false;
            if (cell  && cell->isPoping == false)
            {
                isHaveCell = true;
                cell->preturn = NULL;
            }
            vis[row][col] = isHaveCell;
            //            char buffer[128];
            //            sprintf(buffer, "%3d",isHaveCell);
            //            str+= buffer;
        }
        //        log("vis:%s",str.c_str());
    }
}

void Board::addListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        log("onTouchBegan");
        
        //没有准备好 或者正在更新cellVec
        if(isReady == false || isVecUpdatIng) return false;
        
        auto nodeLocPos = convertToNodeSpace(touch->getLocation());
        int col = nodeLocPos.x / cellWidth + 1;
        int row = ROW - nodeLocPos.y / cellHeight + 1;
        log("row:%d col:%d", row, col);
        
        if (isLegal(row, col))
        {
            auto cell = cellVec[row][col];
            if (preSelCell != cell)
            {
                bool canLink = testLink(cell,preSelCell);
                //如果能够连接
                if (canLink)
                {
                    removeLink(cell,preSelCell);
                }
                else
                {
                    cell->setSelected();
                    if (preSelCell)
                    {
                        //                        log("getScale:     %d",preSelCell->getTag());
                        preSelCell->unSelected();
                    }
                    preSelCell = cell;
                }
            }
        }
        return true;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event)
    {
        //log("onTouchMoved");
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        //log("onTouchEnded");
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Board::loadCell()
{
    MapInfo mapInfo = GameData::getInstance()->getMapInfo();
    vector<vector<int>> map = mapInfo.mapArray;
    cellKinds = mapInfo.kinds;
    auto winSize = Director::getInstance()->getWinSize();
    auto temp = Cell::create(0, 0,0, "fruit_00.png");
    cellWidth = temp->getContentSize().width;
    cellHeight = temp->getContentSize().height;
    maxHeight = cellHeight * maxRow - cellHeight / 2;
    //左右两边有空的墙
    maxCol = (int)map[0].size() - 2;
    //不是空的Cell暂时存储起来
    vector<Cell*> cellArray;
    vector<int>::iterator iter;
    //开辟maxCol+2个空间 便于下标访问
    for (int i = 0; i < maxRow + 2; i++)
    {
        cellVec[i].resize(maxCol + 2);
    }
    
    bool isCreateRand = false;
    int frameIndex = 0;
    for (int row = 1; row <= maxRow; row++)
    {
        for (int col = 1; col <= maxCol; col++)
        {
            int hasCell = map[row][col];
            if (isCreateRand == false)
            {
                //先生成kinds * 2个cell 保证该局一定有kinds种cell
                frameIndex = hasCellCount / 2;
                //当生成kinds种cell时候开始随机  并且保证成对出现
                if (frameIndex >= cellKinds-1) isCreateRand = true;
            }
            else
            {
                bool isRpeat = hasCellCount % 2;
                if (isRpeat == false) frameIndex = rand_0_1() * cellKinds;
            }
            
            if (hasCell == 0)
            {
                cellVec[row].push_back(NULL);
            }
            else
            {
                bool hasNeedAddData = true;
                int frameNum = frameIndex;
                int type = frameNum;
                if(hasCell == -1)
                {
                    hasNeedAddData = false;
                    frameNum = 20;
                    type = -1;
                }
                const char* frameName = String::createWithFormat("fruit_%02d.png", frameNum)->getCString();
                //                auto cell = Cell::create(row, col, type,frameName);
                //                auto size = cell->getContentSize();
                //                float posX = -cellWidth / 2 + col * size.width;
                //                float posY = maxHeight - (row-1) * size.height;
                //                cell->setPosition(Vec2(posX, posY));
                //                cell->setVisible(false);
                auto cell = createCell(row, col, frameNum, type);
                cell->setVisible(false);
                addChild(cell,10);
                if(hasNeedAddData)
                {
                    cellArray.push_back(cell);
                    hasCellCount++;
                }
                cellVec[row][col] = cell;
            }
        }
    }
    
    
    //设置棋盘的位置
    float offX = 1.2f*cellWidth;
    float offY = (winSize.height - cellHeight * maxRow) / 2;
    setPosition(Vec2(offX, offY));
    
    centerPos.x = maxCol * cellWidth / 2 + offX;
    centerPos.y = maxRow * cellHeight / 2 + offY;
    
    //初始化visVec
    for (int i = 0; i <= ROW + 1; i++)
    {
        vis[i].resize(maxCol + 2);
    }
    
    
    //交换cell
    exchangeCells();
    
    debugCellVec();
    
    schedule(schedule_selector(Board::updateReady), 0.8f, 3, 2.0f);
    
    fallToBoard();
}

void Board::fallToBoard()
{
    log("fallToBoard");
    int fallNum = 0;
    float winHeight = Director::getInstance()->getWinSize().height;
    vector<float> delayTimeVec(maxCol+2,0.5f);
    float moveTime = 0.1f;
    for (int i = maxRow; i >= 1; i--)
    {
        bool haveCellCol = false;
        for (int j = 1; j <= maxCol; j++)
        {
            auto cell = cellVec[i][j];
            if(!cell) continue;
            haveCellCol = true;
            delayTimeVec[i] = delayTimeVec[i] +  0.02f;
            auto aimpos = cell->getPosition();
            auto curPos = Vec2(aimpos.x,winHeight + cellHeight);
            cell->setPosition(curPos);
            auto delay = DelayTime::create(delayTimeVec[i]);
            auto fadeIn = FadeIn::create(moveTime);
            auto moveTo = MoveTo::create(moveTime, aimpos);
            auto spawn = Spawn::create(fadeIn,moveTo, NULL);
            auto scaleX1 = ScaleTo::create(0.1f,1.2f,1.0f);
            auto scaleX2 = ScaleTo::create(0.05f,1.0f,1.0f);
            auto moveBy = MoveBy::create(0.05f, Vec2(0,20));
            auto spawn2 = Spawn::create(scaleX2,moveBy, NULL);
            log("runAction:i=%d j=%d  delayTime:%f",i,j,delayTimeVec[i]);
            auto callBack = CallFuncN::create(CC_CALLBACK_1(Board::delay_callBack,this));
            auto callBack2  = CallFuncN::create(CC_CALLBACK_1(Board::fall_callBack,this));
            Sequence* seq = NULL;
            if(fallNum % 3 == 0)
            {
                seq = Sequence::create(delay,callBack,spawn,callBack2,scaleX1,spawn2,moveBy->reverse(),NULL);
            }
            else
            {
                seq = Sequence::create(delay,callBack,spawn,scaleX1,spawn2,moveBy->reverse(),NULL);
            }
            cell->runAction(seq);
            fallNum++;
        }
        if(haveCellCol) delayTimeVec[i-1] = delayTimeVec[i] + moveTime;
        else delayTimeVec[i-1] = delayTimeVec[i];
    }
}

void Board::delay_callBack(cocos2d::Ref *pSender)
{
    ((Node*)pSender)->setVisible(true);
}


void Board::fall_callBack(Node* pSendr)
{
    Audio::getInstance()->playEffect(sound_fall);
}

void Board::debugCellVec()
{
    for (int i = 0; i < maxRow + 2; i++)
    {
        string str = "";
        for (int j = 0; j < maxCol + 2; j++)
        {
            auto cell = cellVec[i][j];
            char buffer[128];
            string temp = "";
            if (cell)
            {
                if(cell->isPoping)
                    sprintf(buffer, "_%dp",cell->type);
                else
                    sprintf(buffer, "%3d",cell->type);
            }
            else
            {
                sprintf(buffer, "  %s", "|");
            }
            temp = buffer;
            str += buffer;
        }
        log("%s",str.c_str());
    }
    
    
    
    //    for (int i = 0; i < maxRow + 2; i++)
    //    {
    //        string str = "";
    //        for (int j = 0; j < maxCol + 2; j++)
    //        {
    //            auto cell = cellVec[i][j];
    //            char buffer[128];
    //            string temp = "";
    //            if (cell)
    //            {
    //                auto precell = cell->preturn;
    //                if(precell)
    //                {
    //                    sprintf(buffer, "[%d,%d]",precell->x,precell->y);
    //                }
    //            }
    //            else
    //            {
    //                sprintf(buffer, "  %s  ", "|");
    //            }
    //            temp = buffer;
    //            str += buffer;
    //        }
    //        log("%s",str.c_str());
    //    }
}

void Board::updateReady(float dt)
{
    countdown --;
    std::stringstream ss;
    ss << countdown;
    auto str = ss.str();
    if(countdown == 0)
    {
        str = "开始!";
        isReady = true;
        Audio::getInstance()->playEffect(sound_startgame);
    }
    else
    {
        
        Audio::getInstance()->playEffect(sound_drop);
    }
    auto label = Text::create(str, "normal.TTF",80);
    label->setColor(Color3B(230,0,29));
    label->setPosition(centerPos + Vec2(0,cellHeight*maxRow));
    label->setScale(50);
    auto moveTo = MoveTo::create(0.1f, centerPos);
    auto scaleTo = ScaleTo::create(0.1f, 1.0f);
    auto removeSelf = RemoveSelf::create();
    auto seq = Sequence::create(Spawn::create(moveTo,scaleTo, NULL),
                                ScaleTo::create(0.05f, 2.0f),
                                ScaleTo::create(0.05f, 1.0f),
                                DelayTime::create(0.5f),
                                removeSelf, NULL);
    label->runAction(seq);
    getParent()->addChild(label,100);
}

bool Board::isLegal(int row, int col)
{
    if (row <= 0 || row > maxRow || col <= 0 || col > maxCol) return false;
    auto cell = cellVec[row][col];
    if (cell == NULL || cell->isPoping || cell->getCellKind() == KIND_WALL) return false;
    return true;
}

bool Board::testLink(Cell* selCell, Cell* preCell)
{
    if (preCell == NULL || selCell == NULL || preCell == selCell || selCell->isPoping || preCell->isPoping || selCell->getCellKind() == KIND_WALL || preCell->getCellKind() == KIND_WALL)
    {
        //log("is NULL");
        return false;
    }
    initVis();//更新vis访问数组
    debugCellVec();
    vis[selCell->x][selCell->y] = true;//起始点访问设置为true
    vis[preCell->x][preCell->y] = false;//目标点可抵达设置为false
    log("start:[%d %d %d]  end:[%d %d %d]", selCell->x, selCell->y,selCell->type, preCell->x, preCell->y,preCell->type);
    //如果两张图片一样
    if (preCell->type == selCell->type)
    {
        step_num = 500;//最多连接500步
        turn_num = 2;//最多有两个拐弯
        link_logic(selCell, preCell);
    }
    
    return is_link;
    
}

void Board::link_logic(Cell* start, Cell* end)
{
    static int step = 0;
    if (start->t > turn_num) return;//如果大于2个弯直接退出
    if (step > step_num)  return; //如果步数大于之前的步数说明不是最优解 直接跳出
    if (start->x == end->x && start->y == end->y)
    {
        is_link = true;
        step_num = MIN(step, step_num);//获取最优解
        turn_num = MIN(start->t, turn_num);
        log("addPath");
        addPath(start);
        //debugTurn(start);//拐点信息
    }
    else
    {
        for (int i = 0; i < 4; i++)//四个方向
        {
            int xx = start->x + dir[i][0];
            int yy = start->y + dir[i][1];
            //在范围内且没有被访问过或者未被阻挡
            if (xx >= 0 && xx <= maxRow + 1 && yy >= 0 && yy <= maxCol + 1 && !vis[xx][yy])
            {
                Cell* temp = Cell::create(xx, yy, start->t, start->preturn);
                //起始点的转弯点为空
                if (start->preturn == NULL)
                {
                    temp->preturn = start;
                }
                else if (start->preturn->x != xx && start->preturn->y != yy)//如果横坐标纵坐标都和上一个拐点的坐标不一致 该点是拐点
                {
                    temp->t += 1;//拐点个数+1
                    temp->preturn = start;
                }
                vis[xx][yy] = true;//被访问
                step++;//步数+1
                link_logic(temp, end);
                vis[xx][yy] = false;//恢复
                step--;//恢复
            }
        }
    }
}

void Board::addPath(Cell* start)
{
    pathVec.setpVec.clear();
    pathVec.turnVec.clear();
    //路径vec
    vector<Vec3>stepVec;
    Cell* temp = start;
    int i = 0;
    while (temp->preturn != NULL)
    {
        //log("temp:%x %d",temp->x,temp->y);
        i++;
        if(i>20)
        {
            temp->preturn;
        }
        for (int i = temp->x; i<temp->preturn->x; i++)
        {
            stepVec.push_back(Vec3(i, temp->y, temp->t));
        }
        for (int i = temp->x; i>temp->preturn->x; i--)
        {
            stepVec.push_back(Vec3(i, temp->y, temp->t));
        }
        for (int j = temp->y; j<temp->preturn->y; j++)
        {
            stepVec.push_back(Vec3(temp->x, j, temp->t));
        }
        for (int j = temp->y; j>temp->preturn->y; j--)
        {
            stepVec.push_back(Vec3(temp->x, j, temp->t));
        }
        temp = temp->preturn;
    }
    stepVec.push_back(Vec3(temp->x, temp->y, temp->t));
    
    //拐点vec
    vector<Vec3>turnVec;
    temp = start;
    turnVec.push_back(Vec3(temp->x, temp->y, temp->t));
    while (temp->preturn != NULL)
    {
        Vec3 point = Vec3(temp->preturn->x, temp->preturn->y, temp->preturn->t);
        turnVec.push_back(point);
        temp = temp->preturn;
    }
    
    pathVec.setpVec = stepVec;
    pathVec.turnVec = turnVec;
    
    allPaths.push_back(pathVec);
    
    log("end");
}


void Board::playParticle(Ref* preCell)
{
    Node* cell = (Node*)preCell;
    auto patricle = ParticleSystemQuad::create("langhua.plist");
    patricle->setPosition(cell->getPosition());
    patricle->setAutoRemoveOnFinish(true);
    addChild(patricle,11);
    //没走一步+1
    GameLayer* game = (GameLayer*)getParent();
    game->menuLayer->updateScore(1);
}

void Board::removeCell_callBack(Ref* preCell)
{
    isMoving = false;
    
    Cell* pCell = (Cell*)preCell;
    //    int row = pCell->x;
    //    int col = pCell->y;
    //    cellVec[row][col] = NULL;
    
    
    Cell* cell = pCell->nextCell;
    //    row = cell->x;
    //    col = cell->y;
    //    cellVec[row][col] = NULL;
    
    
    auto cellPos = cell->getPosition();
    
    cell->removeFromParent();
    pCell->removeFromParent();
    
    reduceCellCount(2,cellPos);
    
    MapInfo mapInfo = GameData::getInstance()->getMapInfo();
    vector<Pos> gravityVec = mapInfo.gravityPosVec;
    //如果需要移动更新cellVec判断是否有可行的匹配在updateVec_callBack中
    if(gravityVec.size() < 2)
    {
        //保证可以交换
        if(hasCellCount>0  && findPairCell().isFind == false)
        {
            exchangeCells(true);
        }
    }
}




//打印路径信息
void Board::debugPath(TurnStepPath pathVec)
{
    vector<Vec3>setpVec = pathVec.setpVec;
    log("{%dSteps  %.fTurns)", (int)setpVec.size(), setpVec[0].z);
    string str = "";
    vector<Vec3>::iterator iter;
    for (iter = setpVec.begin(); iter != setpVec.end(); iter++)
    {
        Vec3 point = *iter;
        char buffer[128];
        sprintf(buffer, "<--[ %.f %.f %.f ]", point.x, point.y, point.z);
        string temp = buffer;
        str += temp;
    }
    str += "}";
    log("%s",str.c_str());
}
//打印拐点信息
void Board::debugTurn(TurnStepPath pathVec)
{
    vector<Vec3>turnVec = pathVec.turnVec;
    log("{%dSteps  %.fTurns)", (int)turnVec.size(), turnVec[0].z);
    string str = "";
    vector<Vec3>::iterator iter;
    for (iter = turnVec.begin(); iter != turnVec.end(); iter++)
    {
        Vec3 point = *iter;
        char buffer[128];
        sprintf(buffer, "<--[ %.f %.f %.f ]", point.x, point.y, point.z);
        string temp = buffer;
        str += temp;
    }
    str += "}";
    log("%s",str.c_str());
}

//自定义排序函数
bool sortBySize1(const TurnStepPath &v1, const TurnStepPath &v2)
{
    vector<Vec3> stepV1 = v1.setpVec;
    vector<Vec3> stepV2 = v2.setpVec;
    //先根据拐弯数目排序
    if (stepV1[0].z != stepV2[0].z) return stepV1[0].z < stepV2[0].z;
    //再根据长度排序
    return stepV1.size() < stepV2.size();
}

TurnStepPath Board::getMinTurnAndStepPath()
{
    sort(allPaths.begin(), allPaths.end(), sortBySize1);
    vector<TurnStepPath>::iterator iter;
    for (iter = allPaths.begin(); iter != allPaths.end(); iter++)
    {
        pathVec = *iter;
        debugPath(pathVec);
        debugTurn(pathVec);
    }
    return allPaths[0];
}

void Board::resetData()
{
    log("resetData");
    is_link = false;
    vector<TurnStepPath>::iterator iter;
    for (iter = allPaths.begin(); iter != allPaths.end(); iter++)
    {
        TurnStepPath temp = *iter;
        temp.setpVec.clear();
        temp.turnVec.clear();
    }
    allPaths.clear();
}

void Board::removeLink(Cell* selCell, Cell* preCell)
{
    isMoving = true;
    
    //用isMoving可以判断如果没有消除完成就不响应触摸但是会影响用户体验
    //设置这个可以并列执行 如果用户触摸正在消除的cell则无效  点击其他的继续处理
    selCell->isPoping = true;
    preCell->isPoping = true;
    
    pathVec = getMinTurnAndStepPath();
    //debugPath(pathVec);
    //debugTurn(pathVec);
    effectLink(selCell,preCell,pathVec);
    
    //把ZoOrder调节高一点避免移动是的时候覆盖掉
    selCell->setZOrder(selCell->getZOrder()+1);
    preCell->setZOrder(preCell->getZOrder()+1);
    //更新cellVec的信息
    int selRow = selCell->x;
    int selCol = selCell->y;
    cellVec[selRow][selCol] = NULL;
    int preRow = preCell->x;
    int preCol = preCell->y;
    cellVec[preRow][preCol] = NULL;
    //移动棋子
    moveCells(selRow, selCol, preRow, preCol);
    //被消除
    preSelCell = NULL;
    resetData();
}

//加载粒子效果
void Board::effectLink(Cell* cell,Cell* preCell,TurnStepPath pathVec)
{
    //vector<Vec3> vec = pathVec.turnVec;
    vector<Vec3> vec = pathVec.setpVec;
    vector<Vec3>::iterator iter;
    Vector<FiniteTimeAction *> arrayOfActions;
    float row = preCell->x;
    float col = preCell->y;
    for (iter = vec.begin(); iter != vec.end(); iter++)
    {
        Vec3 point = *iter;
        float posY = cellWidth * (ROW -point.x) + cellWidth/2;
        float posX = point.y * cellHeight -  cellHeight/2;
        Vec2 pos = Vec2(posX, posY);
        float disRow = point.x - row;
        float disCol = point.y - col;
        float dis = abs(disRow) + abs(disCol);
        //if(dis > 0)
        {
            auto move = MoveTo::create(dis*0.1f,pos);
            auto callBack = CallFuncN::create(CC_CALLBACK_1(Board::playParticle,this));
            arrayOfActions.pushBack(Sequence::create(move,callBack,NULL));
        }
        row = point.x;
        col = point.y;
    }
    log("len%d",(int)arrayOfActions.size());
    //此处标记下消除
    preCell->nextCell = cell;
    auto seqAction = Sequence::create(arrayOfActions);
    auto seqCallBack = Sequence::create(seqAction,CallFuncN::create(CC_CALLBACK_1(Board::removeCell_callBack,this)), NULL);
    auto finalAction = EaseOut::create(seqCallBack, 0.3);
    preCell->runAction(finalAction);
}

Search2Pos Board::findPairCell()
{
    log("findPairCell");
    
    int notNullAndPopingCount = 0;
    
    for(int i=1 ; i<= maxRow; i++ )
    {
        for(int j=1;j<=maxCol;j++)
        {
            Cell* preCell = cellVec[i][j];
            if(preCell && preCell->isPoping == false && preCell->getCellKind() != KIND_WALL )
            {
                notNullAndPopingCount++;
            }
            for(int row = i;row <=maxRow;row++)
            {
                for(int col = 1; col <= maxCol ; col++)
                {
                    if(row == i && col <= j) continue;
                    Cell* nextCell = cellVec[row][col];
                    bool canLink = testLink(nextCell, preCell);
                    if(canLink)
                    {
                        resetData();
                        sea2Pos.selRow = preCell->x;
                        sea2Pos.selCol = preCell->y;
                        sea2Pos.preRow = nextCell->x;
                        sea2Pos.preCol = nextCell->y;
                        sea2Pos.isFind = true;
                        return sea2Pos;
                    }
                }
            }
        }
    }
    
    
    sea2Pos.isFind = false;
    if(notNullAndPopingCount == 0) sea2Pos.isFind = true;
    return sea2Pos;
}

void Board::exchangeCells(bool isPlayAction)
{
    vector<Cell*> tempVec;
    vector<Cell*>::iterator iter;
    for(int i=1 ; i<= maxRow; i++ )
    {
        for(int j=1;j<=maxCol;j++)
        {
            Cell* cell = cellVec[i][j];
            if(cell && cell->canBeExchage())
            {
                tempVec.push_back(cell);
            }
        }
    }
    int cellCount = (int)tempVec.size();
    
    //避免只有四个cell的时候比如
    /********
     0 1
     1 0
     如果都交换的时候就会造成死循环
     所以只有四个cell的时候只需要交换两个就可以必然无解的情况了
     */
    if(cellCount <= 4) cellCount = 2;
    
    float moveTime = 0.5f;
    while (cellCount > 0)
    {
        int index1 = rand_0_1() * cellCount;
        iter = tempVec.begin();
        Cell* c1 = tempVec[index1];
        Vec2 c1Pos = c1->getPosition();
        tempVec.erase(iter+index1);
        
        cellCount -= 1;
        
        int index2 = rand_0_1() * cellCount;
        iter = tempVec.begin();
        Cell* c2 = tempVec[index2];
        Vec2 c2Pos = c2->getPosition();
        tempVec.erase(iter+index2);
        
        cellCount -= 1;
        
        
        //交换在cellVec里的索引位置
        cellVec[c1->x][c1->y] = c2;
        cellVec[c2->x][c2->y] = c1;
        //交换c1 c2的row col
        c1->swap(c2);
        
        //交换position
        if(isPlayAction)
        {
            c1->isPoping = true;
            c2->isPoping = true;
            
            auto mA1 = MoveTo::create(moveTime,c2Pos);
            auto mA2 = MoveTo::create(moveTime,c1Pos);
            c1->runAction(mA1);
            c2->runAction(mA2);
        }
        else
        {
            c1->setPosition(c2Pos);
            c2->setPosition(c1Pos);
        }
    }
    
    if(isPlayAction)
    {
        isMoving = true;
        auto delay = DelayTime::create(moveTime * 1.2f);
        auto callFun = CallFunc::create(CC_CALLBACK_0(Board::exchangeCallBack,this));
        auto seq = Sequence::create(delay,callFun ,NULL);
        runAction(seq);
    }
    else
    {
        static int tryexNum = 0;
        if(findPairCell().isFind == false)
        {
            tryexNum++;
            log("尝试第%d次:",tryexNum);
            if(tryexNum >= 10)
            {
                log("is Dead");
                Toast::toast("你已经无路可走");
//                MessageBox("异常数据请联系1719677034", "err");
                GameLayer* game = (GameLayer*)getParent();
                game->gameOver();
                return;
            }
            exchangeCells();
        }
        else
        {
            tryexNum = 0;
        }
    }
}

void Board::exchangeCallBack()
{
    log("exchangeCallBack");
    //移动结束
    isMoving = false;
    static int tryNum = 0;
    
    
    for(int i=1 ; i<= maxRow; i++ )
    {
        for(int j=1;j<=maxCol;j++)
        {
            Cell* cell = cellVec[i][j];
            if(cell)
            {
                cell->isPoping = false;
            }
        }
    }
    
    if(findPairCell().isFind == false)
    {
        tryNum++;
        log("尝试第%d次:",tryNum);
        if(tryNum >= 10)
        {
            log("is Dead");
            MessageBox("异常数据请联系1719677034", "err");
        }
        else
        {
            exchangeCells();
        }
        
    }
    else
    {
        tryNum = 0;
    }
}

void Board::removeOneType(int type)
{
    isMoving = true;
    //随机删除一种
    if(type == -1)
    {
        vector<int>typeVec(MAX_KINDS,0);
        //余下的类型
        int leftTypes = 0;
        for(int i=1 ; i<= maxRow; i++ )
        {
            for(int j=1;j<=maxCol;j++)
            {
                Cell* cell = cellVec[i][j];
                if(cell && cell->type > 0)typeVec[cell->type]++;
            }
        }
        
        for(int i=0;i<MAX_KINDS;i++)
        {
            if(typeVec[i] > 0) leftTypes++;
        }
        //随机
        int randIndex = rand_0_1() * leftTypes;
        int curIndex = 0;
        for(int i=0;i<MAX_KINDS;i++)
        {
            if(typeVec[i]>0)
            {
                if(curIndex == randIndex)
                {
                    type = i;
                    break;
                }
                curIndex++;
            }
        }
    }
    
    //第一个该种类型的cell
    Cell* firstTypeCell = NULL;
    Cell* firstTypeCellCopy = NULL;
    for(int i=1 ; i<= maxRow; i++ )
    {
        for(int j=1;j<=maxCol;j++)
        {
            int col = j;
            if(i % 2 == 0) col =maxCol- j + 1;
            //蛇形顺序
            Cell* cell = cellVec[i][col];
            if(cell && cell->type == type)
            {
                if(firstTypeCell == NULL)
                {
                    firstTypeCell = cell;
                    firstTypeCellCopy = firstTypeCell;
                }
                else
                {
                    log("have");
                    firstTypeCell->nextCell = cell;
                    cell->nextCell = NULL;
                    firstTypeCell = cell;
                }
                cellVec[i][col] = NULL;
            }
        }
    }
    
    firstTypeCell = firstTypeCellCopy;
    
    Vector<FiniteTimeAction *> arrayOfActions;
    while (firstTypeCellCopy->nextCell)
    {
        Cell* nextCell = firstTypeCellCopy->nextCell;
        float disRow = firstTypeCellCopy->x - nextCell->x;
        float disCol = firstTypeCellCopy->y - nextCell->y;
        float dis = sqrt(disRow * disRow + disCol * disCol);
        float moveTime = dis * 0.05f;
        Vec2 curPos = firstTypeCellCopy->getPosition();
        Vec2 tarPos = nextCell->getPosition();
        ccBezierConfig bez;
        bez.endPosition = tarPos;
        bez.controlPoint_1 = curPos + Vec2(-cellWidth,cellHeight);
        bez.controlPoint_2 = tarPos + Vec2(cellWidth,cellHeight);
        
        FiniteTimeAction* move =  NULL;
        if(dis < 1.1f)
            move = MoveTo::create(moveTime, nextCell->getPosition());
        else
            move = BezierTo::create(moveTime, bez);
        auto callBack = CallFuncN::create(CC_CALLBACK_1(Board::playParticle,this));
        auto pCallBack = CallFuncN::create(CC_CALLBACK_1(Board::playParticle_callBack,this));
        if(nextCell->nextCell)
            arrayOfActions.pushBack(Sequence::create(move,pCallBack,NULL));
        else
            arrayOfActions.pushBack(Sequence::create(move,callBack,pCallBack,NULL));
        
        firstTypeCellCopy = nextCell;
    }
    
    log("len%d",(int)arrayOfActions.size());
    if(arrayOfActions.size() < 1)
    {
        log("len%d",(int)arrayOfActions.size());
    }
    auto seqAction = Sequence::create(arrayOfActions);
    auto seqCallBack = Sequence::create(seqAction,CallFuncN::create(CC_CALLBACK_1(Board::removeOneType_callBack,this)), NULL);
    firstTypeCell->setLocalZOrder(12);
    firstTypeCell->runAction(seqCallBack);
    firstTypeCell->addParticle(particle_path);
}

void Board::playParticle_callBack(cocos2d::Ref *preCell)
{
    
    Cell* cell = (Cell*)preCell;
    Cell* nextCell = cell->nextCell;
    Cell* nextNextCell = nextCell->nextCell;
    cell->nextCell = nextNextCell;
    
    nextCell->removeFromParent();
    reduceCellCount(1);
}

void Board::removeOneType_callBack(cocos2d::Ref *preCell)
{
    log("removeOneType_callBack");
    isMoving = false;
    Cell* cell = (Cell*)preCell;
    cell->removeFromParent();
    reduceCellCount(1);
}

void Board::removeOnePairCell()
{
    sea2Pos = findPairCell();
    if(sea2Pos.isFind)
    {
        Cell* cell = cellVec[sea2Pos.selRow][sea2Pos.selCol];
        Cell* preCell = cellVec[sea2Pos.preRow][sea2Pos.preCol];
        //恢复起始点的转弯点为空
        //cell->preturn = NULL;
        testLink(cell,preCell);
        removeLink(cell,preCell);
    }
    else
    {
        MessageBox("没有可以消除的了", "err");
    }
}

void Board::reduceCellCount(int subNum , Vec2 cellpos)
{
    Audio::getInstance()->playEffect(sound_pop);
    hasCellCount -= subNum;
    log("hasCellCount:%d", hasCellCount);
    auto pos = cellpos + this->getPosition();
    GameLayer* game = (GameLayer*)getParent();
    game->menuLayer->updateLeftCellsCount(hasCellCount);
    if(cellpos.equals(Vec2(0,0)) == false)game->menuLayer->setSerialLink(pos);
    game->menuLayer->updateScore(subNum);
    if (hasCellCount == 0)
    {
        game->setGameState(Game_Success);
        game->gameOver();
    }
    
    
    int testCount = 0;
    for (int i = 0; i < maxRow + 2; i++)
    {
        for (int j = 0; j < maxCol + 2; j++)
        {
            auto cell = cellVec[i][j];
            if (cell)
            {
                testCount++;
            }
        }
    }
    if(testCount % 2 == 1)
    {
        log("testCount:%d",testCount);
        log("err");
    }
    
}

void Board::debugTestData()
{
    log("debugTestData");
    Cell* selCell = Cell::create(8, 7, 0);
    Cell* preCell = Cell::create(1, 1, 0);
    
    auto  _127cell = Cell::create(1, 2, 7);
    auto  _176cell = Cell::create(1, 7, 6);
    auto  _686cell = Cell::create(6, 8, 6);
    auto  _187cell = Cell::create(1, 8, 7);
    
    for (int i = 0; i < maxRow + 2; i++)
    {
        string str = "";
        for (int j = 0; j < maxCol + 2; j++)
        {
            cellVec[i][j] = NULL;
        }
    }
    
    cellVec[8][7] = selCell;
    cellVec[1][1] = preCell;
    
    cellVec[1][2] = _127cell;
    cellVec[1][7] = _176cell;
    cellVec[6][8] = _686cell;
    cellVec[1][8] = _187cell;
    
    testLink(selCell,preCell);
}

Vec2 Board::getPositionByRowCol(int row, int col)
{
    
    float posX = (col - 0.5f) * cellWidth;
    float posY = (maxRow - row + 0.5f) * cellHeight;
    return Vec2(posX,posY);
}

vector<Vec2> Board::getEmptyVec()
{
    vector<Vec2> tempVec;
    for(int i=1 ; i<= maxRow; i++ )
    {
        for(int j=1;j<=maxCol;j++)
        {
            Cell* cell = cellVec[i][j];
            if(cell == NULL)
            {
                tempVec.push_back(Vec2(i,j));
            }
        }
    }
    return tempVec;
}

void Board::createNextCells(int pairNums , int maxTypeNum)
{
    vector<Vec2> emptyVec = getEmptyVec();
    random_shuffle(emptyVec.begin (), emptyVec.end ());
    int createNum = 0;
    int frameNum = 0;
    for (vector<Vec2>::iterator iter = emptyVec.begin(); iter != emptyVec.end(); iter++)
    {
        auto pos = *iter;
        int row = pos.x + 0.1f;
        int col = pos.y + 0.1f;
        if(createNum % 2 ==0)frameNum = rand_0_1() * maxTypeNum;
        int type = frameNum;
        auto cell = createCell(row, col,frameNum,type);
        cell->setScale(0.5f);
        cell->runAction(ScaleTo::create(0.8f, 1));
        addChild(cell,10);
        hasCellCount++;
        cellVec[row][col] = cell;
        createNum++;
        if(createNum >= 2 * pairNums)
        {
            break;
        }
    }
}

Cell* Board::createCell(int row , int col , int frameNum , int type )
{
    const char* frameName = String::createWithFormat("fruit_%02d.png", frameNum)->getCString();
    auto cell = Cell::create(row, col, type,frameName);
    auto size = cell->getContentSize();
    float posX = -cellWidth / 2 + col * size.width;
    float posY = maxHeight - (row-1) * size.height;
    cell->setPosition(Vec2(posX, posY));
    return cell;
}

void Board::moveCells(int row1 , int col1 , int row2 , int col2)
{
    MapInfo mapInfo = GameData::getInstance()->getMapInfo();
    vector<Pos> gravityVec = mapInfo.gravityPosVec;
    if(gravityVec.size() < 2) return;
    
    isVecUpdatIng = true;
    float moveTime = 0.1f;
    map<Cell*, Vec2> movePosMap;
    
    Pos gravityPos = gravityVec[0];
    Pos dirPos = gravityVec[1];
    int gRow = gravityPos.x;
    int gCol = gravityPos.y;
    int dir1 = dirPos.x;
    int dir2 = dirPos.y;
    bool isInSameMoveLine = false;
    bool isInSameRowLine = (row1 == row2);
    bool isInSameColLine = (col1 == col2);
    
    int minRow1 = min(row1,row2);
    int maxRow2 = max(row1,row2);
    int minCol1 = min(col1,col2);
    int maxCol2 = max(col1,col2);
    if(isInSameRowLine)
    {
        //左右移动
        if(gRow == 0)
        {
            //往右移动
            if(maxCol2 <= gCol)
            {
                isInSameMoveLine = true;
            }
            //往左移动
            if(minCol1 > gCol)
            {
                isInSameMoveLine = true;
            }
        }
    }
    
    if(isInSameColLine)
    {
        //上下移动
        if(gCol == 0)
        {
            //往下移动
            if(maxRow2 <= gRow)
            {
                isInSameMoveLine = true;
            }
            //往上移动
            if(minRow1 > gRow)
            {
                isInSameMoveLine = true;
            }
        }
    }
    
    //如果不在同一个消除线上
    if(isInSameMoveLine == false)
    {
        moveCells(row1, col1 , movePosMap);
        moveCells(row2, col2 , movePosMap);
    }
    else
    {
        if(gRow == 0)
        {
            
            gCol = MAX(gCol,0);
            gCol = MIN(gCol,maxCol);
            
            //此时row1是等于row2的 因为isInSameMoveLine是true
            int referRow = row1;
            //在[maxCol2 minCol1]之前是否有不能移动的Cell
            bool isHaveCanNotMoveCellBtw = false;
            //移动单位
            int moveUnit = 1;
            //左半部分
            if(maxCol2 <= gCol)
            {
                //往右移动
                if(dir1 == 1)
                {
                    //[maxCol2 , minCol1]之间移动一个单位
                    for (int col = maxCol2-1; col > minCol1 ; col--)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = referRow;
                            cell->y = col+moveUnit;
                            Vec2 movePos = Vec2(moveUnit*cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[referRow][col+moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                    //isHaveCanNotMoveCellBtw为假时候[gCol minCol1]之间移动两个单位否则一个单位
                    int endCol = 1;
                    for (int col = minCol1-1; col >= endCol ; col--)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = referRow;
                            cell->y = col+moveUnit;
                            Vec2 movePos = Vec2(moveUnit*cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[referRow][col+moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                }
                
                //往左移动
                if(dir1 == -1)
                {
                    //此时row1是等于row2的 因为isInSameMoveLine是true
                    int referRow = row1;
                    //在[maxCol2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    
                    for(int col=minCol1+1;col<maxCol2;col++)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = referRow;
                            cell->y = col-moveUnit;
                            Vec2 movePos = Vec2(-moveUnit * cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向左移动
                            cellVec[referRow][col-moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                    int endCol = gCol;
                    //isHaveCanNotMoveCellBtw为假时候[gCol maxCol2]之间移动两个单位否则一个单位
                    for(int col = maxCol2+1 ; col <=  endCol ; col++)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = referRow;
                            cell->y = col-moveUnit;
                            Vec2 movePos = Vec2(-moveUnit * cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向左移动
                            cellVec[referRow][col-moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                        
                    }
                }
            }
            
            //右半部分
            if(minCol1 > gCol)
            {
                //往右移动
                if(dir2 == 1)
                {
                    //[maxCol2 , minCol1]之间移动一个单位
                    for (int col = maxCol2-1; col > minCol1 ; col--)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = referRow;
                            cell->y = col+moveUnit;
                            Vec2 movePos = Vec2(moveUnit*cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[referRow][col+moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                    //isHaveCanNotMoveCellBtw为假时候[gCol minCol1]之间移动两个单位否则一个单位
                    //左半部分
                    int endCol = gCol;
                    for (int col = minCol1-1; col >= endCol ; col--)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = referRow;
                            cell->y = col+moveUnit;
                            Vec2 movePos = Vec2(moveUnit*cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[referRow][col+moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                }
                
                if(dir2 == -1)
                {
                    //此时row1是等于row2的 因为isInSameMoveLine是true
                    int referRow = row1;
                    //在[maxCol2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    
                    for(int col=minCol1+1;col<maxCol2;col++)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = referRow;
                            cell->y = col-moveUnit;
                            Vec2 movePos = Vec2(-moveUnit * cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向左移动
                            cellVec[referRow][col-moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                    }
                    int endCol = maxCol;
                    //isHaveCanNotMoveCellBtw为假时候[gCol maxCol2]之间移动两个单位否则一个单位
                    for(int col = maxCol2+1 ; col <=  endCol ; col++)
                    {
                        auto cell = cellVec[referRow][col];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = referRow;
                            cell->y = col-moveUnit;
                            Vec2 movePos = Vec2(-moveUnit * cellWidth,0);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向左移动
                            cellVec[referRow][col-moveUnit] = cell;
                            //置空
                            cellVec[referRow][col] = NULL;
                        }
                        
                    }
                }
            }
        }
        //上下移动
        else if(gCol == 0)
        {
            gRow = MAX(gRow,0);
            gRow = MIN(gRow,maxRow);
            
            if(maxRow2 <= gRow)
            {
                //往下移动
                if(dir1 == 1)
                {
                    //此时col1是等于col2的 因为isInSameMoveLine是true
                    int referCol = col1;
                    //在[maxRow2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    for(int row=maxRow2-1;row>minRow1;row--)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = row+moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,-moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row+moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                    
                    //isHaveCanNotMoveCellBtw为假时候[gCol maxCol2]之间移动两个单位否则一个单位
                    //上半部分
                    int endRow = 1;
                    for(int row = minRow1-1 ; row >= endRow;row--)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = row+moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,-moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row+moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                }
                //往上运动
                if(dir1 == -1)
                {
                    //此时col1是等于col2的 因为isInSameMoveLine是true
                    int referCol = col1;
                    //在[maxRow2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    
                    for(int row=minRow1+1;row<=maxRow2;row++)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = row-moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row-moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                    
                    
                    //下半部分
                    int endRow = gRow;
                    for(int row=maxRow2+1;row<=endRow;row++)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = row-moveUnit;
                            cell->y = referCol;
                            Vec2 toPos = getPositionByRowCol(row-moveUnit, referCol);
                            Vec2 movePos = Vec2(0,moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row-moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                }
            }
            if(minRow1 > gRow)
            {
                //往下移动
                if(dir2 == 1)
                {
                    //此时col1是等于col2的 因为isInSameMoveLine是true
                    int referCol = col1;
                    //在[maxRow2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    for(int row=maxRow2-1;row>minRow1;row--)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = row+moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,-moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row+moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                    
                    //isHaveCanNotMoveCellBtw为假时候[gCol maxCol2]之间移动两个单位否则一个单位
                    //上半部分
                    int endRow = gRow;
                    for(int row = minRow1-1 ; row >= endRow;row--)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = row+moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,-moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row+moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                }
                //往上运动
                if(dir2 == -1)
                {
                    //此时col1是等于col2的 因为isInSameMoveLine是true
                    int referCol = col1;
                    //在[maxRow2 minCol1]之前是否有不能移动的Cell
                    bool isHaveCanNotMoveCellBtw = false;
                    //移动单位
                    int moveUnit = 1;
                    
                    for(int row=minRow1+1;row<=maxRow2;row++)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                isHaveCanNotMoveCellBtw = true;
                                break;
                            }
                            cell->x = row-moveUnit;
                            cell->y = referCol;
                            Vec2 movePos = Vec2(0,moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row-moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                    
                    
                    //下半部分
                    int endRow = maxRow;
                    for(int row=maxRow2+1;row<=endRow;row++)
                    {
                        auto cell = cellVec[row][referCol];
                        if(cell)
                        {
                            if(cell->getCellKind() != KIND_NORMAL)
                            {
                                break;
                            }
                            
                            if(isHaveCanNotMoveCellBtw)
                            {
                                moveUnit = 1;
                            }
                            else
                            {
                                moveUnit = 2;
                            }
                            cell->x = row-moveUnit;
                            cell->y = referCol;
                            Vec2 toPos = getPositionByRowCol(row-moveUnit, referCol);
                            Vec2 movePos = Vec2(0,moveUnit*cellHeight);
                            movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                            //向右移动
                            cellVec[row-moveUnit][referCol] = cell;
                            //置空
                            cellVec[row][referCol] = NULL;
                        }
                    }
                }
            }
        }
    }
    
    
    
    log("movePosMapSIize:%lu",movePosMap.size());
    map<Cell*, Vec2>::iterator iter;
    for(iter = movePosMap.begin(); iter != movePosMap.end(); iter++)
    {
        auto cell = iter->first;
        auto movePos = iter->second;
        cell->runAction(MoveBy::create(moveTime, movePos));
    }
    this->scheduleOnce(schedule_selector(Board::updateVec_callBack), moveTime);
}

//更新cellVec里面的内容是瞬时完成的  与removeCell_callBack检测是否有可行的匹配不影响
void Board::moveCells(int referRow , int referCol , map<Cell*, Vec2> &movePosMap)
{
    MapInfo mapInfo = GameData::getInstance()->getMapInfo();
    vector<Pos> gravityVec = mapInfo.gravityPosVec;
    Pos gravityPos = gravityVec[0];
    Pos dirPos = gravityVec[1];
    int gRow = gravityPos.x;
    int gCol = gravityPos.y;
    int dir1 = dirPos.x;
    int dir2 = dirPos.y;
    
    //左右移动
    if(gRow == 0)
    {
        gCol = MAX(gCol,0);
        gCol = MIN(gCol,maxCol);
        
        //往右移动
        if(referCol <= gCol)
        {
            if(dir1 == 1)
            {
                //左半部分
                int endCol = 1;
                for(int col=referCol-1;col>=endCol;col--)
                {
                    auto cell = cellVec[referRow][col];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(referRow, col+1);
                        cell->x = referRow;
                        cell->y = col+1;
                        Vec2 movePos = Vec2(cellWidth,0);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向右移动
                        cellVec[referRow][col+1] = cell;
                        //置空
                        cellVec[referRow][col] = NULL;
                    }
                }
            }
            
            //往左移动
            if(dir1 == -1)
            {
                //左半部分
                int endCol = gCol;
                //右半部分
                for(int col=referCol+1;col<=endCol;col++)
                {
                    auto cell = cellVec[referRow][col];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(referRow, col-1);
                        cell->x = referRow;
                        cell->y = col-1;
                        Vec2 movePos = Vec2(-cellWidth,0);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向左移动
                        cellVec[referRow][col-1] = cell;
                        //置空
                        cellVec[referRow][col] = NULL;
                    }
                }
            }
        }
        
        //右半部分
        if(referCol > gCol)
        {
            //向右移动
            if(dir2 == 1)
            {
                //左半部分
                int endCol = gCol;
                for(int col=referCol-1;col>endCol;col--)
                {
                    auto cell = cellVec[referRow][col];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(referRow, col+1);
                        cell->x = referRow;
                        cell->y = col+1;
                        Vec2 movePos = Vec2(cellWidth,0);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向右移动
                        cellVec[referRow][col+1] = cell;
                        //置空
                        cellVec[referRow][col] = NULL;
                    }
                }
            }
            
            
            //往左移动
            if(dir2 == -1)
            {
                int endCol = maxCol;
                for(int col=referCol+1;col<=endCol;col++)
                {
                    auto cell = cellVec[referRow][col];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(referRow, col-1);
                        cell->x = referRow;
                        cell->y = col-1;
                        Vec2 movePos = Vec2(-cellWidth,0);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向左移动
                        cellVec[referRow][col-1] = cell;
                        //置空
                        cellVec[referRow][col] = NULL;
                    }
                }
            }
        }
    }
    //上下移动
    else if(gCol == 0)
    {
        gRow = MAX(gRow,0);
        gRow = MIN(gRow,maxCol);
        
        
        if(referRow <= gRow)
        {
            //往下移动
            if(dir1 == 1)
            {
                //上半部分
                int endRow = 1;
                for(int row=referRow-1;row>=endRow;row--)
                {
                    auto cell = cellVec[row][referCol];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(row+1, referCol);
                        cell->x = row+1;
                        cell->y = referCol;
                        Vec2 movePos = Vec2(0,-cellHeight);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向下移动
                        cellVec[row+1][referCol] = cell;
                        //置空
                        cellVec[row][referCol] = NULL;
                    }
                }
            }
            
            //往上移动
            if(dir1 == -1)
            {
                int endRow = gRow;
                for(int row=referRow+1;row<=endRow;row++)
                {
                    auto cell = cellVec[row][referCol];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        cell->x = row-1;
                        cell->y = referCol;
                        Vec2 toPos = getPositionByRowCol(row-1, referCol);
                        Vec2 movePos = Vec2(0,cellHeight);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向上移动
                        cellVec[row-1][referCol] = cell;
                        //置空
                        cellVec[row][referCol] = NULL;
                    }
                }
            }
        }
        if(referRow > gRow)
        {
            //往下移动
            if(dir2 == 1)
            {
                //上半部分
                int endRow = gRow;
                for(int row=referRow-1;row>endRow;row--)
                {
                    auto cell = cellVec[row][referCol];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        Vec2 toPos = getPositionByRowCol(row+1, referCol);
                        cell->x = row+1;
                        cell->y = referCol;
                        Vec2 movePos = Vec2(0,-cellHeight);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向下移动
                        cellVec[row+1][referCol] = cell;
                        //置空
                        cellVec[row][referCol] = NULL;
                    }
                }
            }
            
            if(dir2 == -1)
            {
                //往上移动
                int endRow = maxRow;
                for(int row=referRow+1;row<=endRow;row++)
                {
                    auto cell = cellVec[row][referCol];
                    if(cell)
                    {
                        if(cell->getCellKind() != KIND_NORMAL) return;
                        cell->x = row-1;
                        cell->y = referCol;
                        Vec2 toPos = getPositionByRowCol(row-1, referCol);
                        Vec2 movePos = Vec2(0,cellHeight);
                        movePosMap.insert(pair<Cell*, Vec2>(cell,movePos));
                        //向上移动
                        cellVec[row-1][referCol] = cell;
                        //置空
                        cellVec[row][referCol] = NULL;
                    }
                }
            }
        }
    }
}

void Board::updateVec_callBack(float dt)
{
    log("updateVec_callBack");
    isVecUpdatIng = false;
    if(hasCellCount>0  && findPairCell().isFind == false)
    {
        exchangeCells(true);
    }
}