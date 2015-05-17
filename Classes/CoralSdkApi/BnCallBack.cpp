#include "BnCallBack.h"
#include "BnInterface.h"


BnCallBack::BnCallBack()
{
    
}

BnCallBack::~BnCallBack()
{
    
}

//参加游戏的人员回调（数组包含userkey,username,gamescore,gamedata）
void BnCallBack::BnGameUserSelectedNotification(CCArray *userlist)
{
    //BnHideSDK();//隐藏SDK
    //进入游戏过程
}

//退出登陆通知
void BnCallBack::BnLogoutNotify()
{
    // BnHideSDK();//隐藏SDK
     //退出登陆，返回到登陆界面
}

//音乐开关通知
void BnCallBack::BnMusicTurnOffAndOn(bool isOn)
{

	 
}

//音效开关通知
void BnCallBack::BnSoundTurnOffAndOn(bool isOn)
{

}

//登录回调:1为登陆成功，2为失败，3为取消登录
void BnCallBack::BnLoginResult(int isLogin)
{

	 
}

//游戏结束后返回按键回调
void BnCallBack::BnGameBackNotification()
{
    //BnHideSDK();//隐藏SDK
    //返回游戏排行榜
}


