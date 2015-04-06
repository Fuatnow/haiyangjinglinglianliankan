#include "Cell.h"
Cell* Cell::create(int xx, int yy, int type, const char* frameName)
{
    Cell* cell = new Cell;
    if (cell && cell->init(xx, yy, type, frameName))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

Cell* Cell::create(int xx, int yy, int type, Cell* turnPoint)
{
    Cell* cell = new Cell;
    if (cell && cell->init(xx, yy, type, turnPoint))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

Cell* Cell::create(int xx, int yy, bool isNullCell)
{
    Cell* cell = new Cell;
    cell->isNullCell = isNullCell;
    cell->init(xx, yy, -1, "fruit_19.png");
    cell->autorelease();
    return cell;
}

bool Cell::init(int xx, int yy, int ttype, const char* frameName)
{
    Sprite::init();
    x = xx;
    y = yy;
    type = ttype;
    t = 0;
    isPoping = false;
    preturn = NULL;
    nextCell = NULL;
    initData();
    if (strlen(frameName) > 0)
    {
        initWithSpriteFrameName(frameName);
    }
    return true;
}

bool Cell::init(int xx, int yy, int tt, Cell* turnPoint)
{
    Sprite::init();
    x = xx;
    y = yy;
    t = tt;
    preturn = turnPoint;
    type = 0;
    isPoping = false;
    return true;
}

void Cell::initData()
{
    //setOpacity(10);
}

void Cell::setSelected()
{
    CCLOG("setSelected");
    auto scale1 = ScaleTo::create(0.5f, 0.8f);
    auto scale2 = ScaleTo::create(0.5, 1.0f);
    auto seq = Sequence::create(scale1, scale2, NULL);
    runAction(RepeatForever::create(seq));
}

void Cell::unSelected()
{
    CCLOG("unSelected");
    setScale(1);
    stopAllActions();
}

void Cell::swap(Cell* cell)
{
    //交换两个贴图 和 类型
//    auto curFrame = this->getSpriteFrame();
//    int curType = this->type;
//    auto aimFrame = cell->getSpriteFrame();
//    int aimType = cell->type;
//    this->setDisplayFrame(aimFrame);
//    this->type = aimType;
//    cell->setDisplayFrame(curFrame);
//    cell->type = curType;
    int row = x;
    int col = y;
    int aimRow = cell->x;
    int aimCol = cell->y;
    x = aimRow;
    y = aimCol;
    cell->x = row;
    cell->y = col;
}

void Cell::addParticle(std::string fileStr)
{
    auto patricle = ParticleSystemQuad::create(fileStr);
    patricle->setPosition(getContentSize()/2);
    patricle->setAutoRemoveOnFinish(true);
    addChild(patricle,-1);
}