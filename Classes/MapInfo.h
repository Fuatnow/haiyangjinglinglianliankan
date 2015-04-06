//
//  MapInfo.h
//  Link
//
//  Created by Fuatnow on 15-4-5.
//
//

#ifndef __Link__MapInfo__
#define __Link__MapInfo__

#include <vector>
#include <iostream>
using namespace std;

struct Pos
{
    int x;
    int y;
};


class AimPassInfo
{
public:
    int passType;
    string discription;
    float lenTime;
    float shortTime;
};

class MapInfo
{
public:
    int themeType;
    int bg;
    int music;
    int kinds;
    int mapRow;
    int mapCol;
    int cellWidth;
    int cellHeight;
    bool isIncrease;
    int unLockType;
    vector<Pos> gravityPosVec;
    vector<vector<int>> mapArray;
    AimPassInfo aimpassInfo;
};

#endif /* defined(__Link__MapInfo__) */