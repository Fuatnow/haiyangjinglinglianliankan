//
//  IosPlatform.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-8.
//
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IosPlatform.h"
Platform* getDevicePlatform()
{
    return new IosPlatform;
}

void IosPlatform::platform(CMD_ID state,int infoNum,string infoStr)
{
    log("IosPlatform");
    switch (state)
    {
        case SHOW_KTPLAY:
            
            break;
            
        default:
            break;
    }
}
string IosPlatform::platform(string jsonStr)
{
    log("%s",jsonStr.c_str());
    string str = "";
    return str;
}

bool IosPlatform::isLogin()
{
    return false;
}
#endif
