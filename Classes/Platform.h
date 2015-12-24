//
//  Platform.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-7.
//
//

#ifndef __SeaLink__Platform__
#define __SeaLink__Platform__
#include "cocos2d.h"
#include "../cocos2d/external/json/rapidjson.h"
#include "../cocos2d/external/json/document.h"
#include "../cocos2d/external/json/writer.h"
#include "../cocos2d/external/json/stringbuffer.h"
#include "../cocos2d/external/libMobClickCpp/include/MobClickCpp.h"
#include "C2DXShareSDK/C2DXShareSDK.h"
using namespace cn::sharesdk;
using namespace rapidjson;
USING_NS_CC;
using namespace std;


enum CMD_ID
{
    SHOW_KTPLAY,
    DISPATCH_REWARD,
    STATUS_CHANGED,
    AVAILABILITY_CHANGED,
    SHOW_INTERSTITIAL_NOTIFICATION,
    DEEP_LINNK,
    LOGINWITH_GAMEUSE,
    SET_NiCKNAME,
    SHOW_LOGINVIEW,
    LOGIN_STATUSCHANGED,
    IS_LOGGEDIN,
    USE_RPROFILE,
    CURRENT_ACCOUNT,
    LOGOUT,
    SHOW_FRIEND_VIEW,
    SEND_FRIEND_REQUEST,
    FRIEND_LIST,
    FRIEND_LEADER_BOARD,
    GLOBAL_LEADER_BOARD,
    REPORT_SCORE,
    KTPLAY_DIDAPPEAR,
    KTPLAY_DISAPPEAR,
    KTPLAY_DISMISS,
    IS_KTSHOWING,
};



class Platform
{
public:
    static Platform* getInstance();
    virtual void platform(CMD_ID state,int infoNum=0,string infoStr="");
    virtual string platform(string jsonStr);
    virtual bool isLogin();
};

extern Platform* getDevicePlatform();
#endif /* defined(__SeaLink__Platform__) */
