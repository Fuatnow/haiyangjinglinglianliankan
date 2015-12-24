//
//  Platform.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-7.
//
//

#include "Platform.h"
static Platform* instance = NULL;
Platform* Platform::getInstance()
{
    return (instance == NULL)  ?  getDevicePlatform() : instance;
}
void Platform::platform(CMD_ID state,int infoNum,string infoStr)
{
    log("parent platform(II)V");
}
string Platform::platform(string jsonStr)
{
    log("parent platform");
    return "not fond device";
};

bool Platform::isLogin()
{
    return false;
}