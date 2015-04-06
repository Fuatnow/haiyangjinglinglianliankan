#ifndef __Cell_H__
#define __Cell_H__
#include "BasicLayer.h"
class Cell : public Sprite
{
public:
    Cell(){};
    ~Cell(){};
    static Cell* create(int xx, int yy, int type, const char* frameName = "");
    static Cell* create(int xx, int yy, int tt, Cell* turnPoint);
    static Cell* create(int xx, int yy, bool isNullCell);
    virtual bool init(int xx, int yy, int type, const char* frameName);
    virtual bool init(int xx, int yy, int tt, Cell* turnPoint);
    void initData();
    void unSelected();
    void setSelected();
    void swap(Cell* cell);
    void addParticle(std::string fileStr);
public:
    int x, y;//坐标
    int t;//拐点个数
    int type;//图片类型
    bool isPoping;//是否正在做消除特效或者其他运动
    bool isNullCell;//是否是一个空的Cell
    Cell* preturn;//上一个拐点
    Cell* nextCell;//标记下一个点
};
#endif
