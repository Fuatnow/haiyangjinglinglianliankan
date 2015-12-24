//
//  AndroidPlatform.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-7.
//
//

#ifndef __SeaLink__AndroidPlatform__
#define __SeaLink__AndroidPlatform__
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#define CLASS_NAME "com/fuatnow/game/sealink/AppActivity"
#include "cocos2d.h"
#include "Platform.h"
USING_NS_CC;
class AndroidPlatform : public Platform
{
public:
    AndroidPlatform(){};
    ~AndroidPlatform(){};
    //static AndroidPlatform* getInstance();
    virtual void platform(CMD_ID state,int infoNum=0,string infoStr="");
    virtual string platform(string jsonStr);
    virtual bool isLogin();
};

#endif
#endif /* defined(__SeaLink__AndroidPlatform__) */
