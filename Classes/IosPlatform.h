//
//  IosPlatform.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-8.
//
//

#ifndef __SeaLink__IosPlatform__
#define __SeaLink__IosPlatform__
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "cocos2d.h"
#include "Platform.h"
USING_NS_CC;
using namespace std;
class IosPlatform : public Platform
{
public:
    IosPlatform(){};
    ~IosPlatform(){};
    //static AndroidPlatform* getInstance();
    virtual void platform(CMD_ID state,int infoNum=0,string infoStr="");
    virtual string platform(string jsonStr);
    virtual bool isLogin();
};

#endif
#endif /* defined(__SeaLink__IosPlatform__) */
