//
//  AndroidPlatform.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-7.
//
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AndroidPlatform.h"
#include "GameData.h"
#include "KTPlayC.h"
#include "KTAccountManagerC.h"
#include "KTErrorC.h"
#include "KTFriendshipC.h"
#include "KTLeaderboardC.h"
#include "KTLeaderboardPaginatorC.h"
#include "KTUserC.h"
#include "RankLayer.h"
#include "ShopLayer.h"
#include "HomeLayer.h"
#include "Toast.h"
#include "GlobalNode.h"
#include "AppDelegate.h"
Platform* getDevicePlatform()
{
    return new AndroidPlatform;
}


void dispatchRewards (KTRewardItemC * pRewardArray, int length)
{
    for ( int i = 0; i < length; i ++)
    {
        if (strcmp(pRewardArray[i].typeId, "Gold") == 0)
        {
        	int coin = pRewardArray[i].value;
        	GameData::getInstance()->addCoin(coin);
        	umeng::MobClickCpp::bonus(coin, 1);
            log("%s %d",pRewardArray[i].typeId,pRewardArray[i].value);
        } else if (strcmp(pRewardArray[i].typeId, "Diamond") == 0)
        {

        	int diamond = pRewardArray[i].value;
			GameData::getInstance()->addDiamond(diamond);
           // Game.appendDiamond( pRewardArray[i].typeId.value)
			umeng::MobClickCpp::bonus(diamond, 1);
        	log("%s %d",pRewardArray[i].typeId,pRewardArray[i].value);
        }
    }
}


void activityStatusChanged(bool hasNewActivities)
{
    if(hasNewActivities)
    {
        GlobalNode* board = (GlobalNode*)Director::getInstance()->getNotificationNode();
        board->newNotice();
    }
    else
    {
    	GlobalNode* board = (GlobalNode*)Director::getInstance()->getNotificationNode();
    	board->hideNotice();
    }
}


void availabilityChangedCallback(bool isEnabled)
{
    //isEnabled 为当前KTPlay实时状态
}


void deepLinkCallback (const char *linkSchemas)
{
	log("%s",linkSchemas);
//	MessageBox(linkSchemas,"deepLinkCallback");
//    Game::processDeepLink(linkSchemas);
//    //linkSchemas内容为"GoToLeaderboard"，游戏接收到此字符串后，跳转到游戏排行版界面
}


static void loginWithGameUserCallback(bool isSuccess ,const char * gameUserid, KTUserC * user,KTErrorC *error)
{
    if (isSuccess)
    {
    	log("login success:%s",gameUserid);
        // 操作成功 user 是当前用户信息

    } else
    {
        // 操作失败
    	log("login faild:%s",error->description);
    }
}

static void setNickNameCallBack(bool isSuccess ,const char * nickName,KTUserC * user, KTErrorC *error)
{
    if (isSuccess) {
        //  user  是当前用户信息
    } else {
        // error 详细错误信息
    }
}

static void showLoginViewCallback(bool isSuccess ,KTUserC * user,KTErrorC *error)
{
    if (isSuccess) {
        //登录成功
    } else {
       //登录失败
    }
}

//创建回调
void loginStatusChangedCallback(bool isLoggedIn, KTUserC * user)
{
    if(isLoggedIn)
    {
        //登录
    	string channelID = GameData::getInstance()->getAppChanel();
    	if(channelID.compare("1")) channelID = "360";
    	if(channelID.compare("2")) channelID = "百度";
    	if(channelID.compare("3")) channelID = "应用宝";
    	umeng::MobClickCpp::Sex sex = umeng::MobClickCpp::Sex::Unkonwn;
    	if(user->gender == 1 ) sex = umeng::MobClickCpp::Sex::Female;
    	if(user->gender == 2 ) sex = umeng::MobClickCpp::Sex::Male;
    	umeng::MobClickCpp::setUserInfo(user->userId, sex, 0, channelID.c_str());
    	log("login");
    } else
    {
         //登出
    	log("logout");
    }
}

static void userProfileCallback(bool isSuccess ,const char *userId ,KTUserC * user,KTErrorC *error)
{
    if (isSuccess)
    {
        // 操作成功 user即玩家信息
    } else {
       //操作失败
    }
}


static void friendLeaderboardCallback(bool isSuccess,const char *leaderboardId ,KTLeaderboardPaginatorC *leaderboard,KTErrorC *error)
{
	if (isSuccess)
	{
		log("isSuccess");
		 //操作成功
		int itemCount = leaderboard->itemCount;
		log("itemCount:%d",itemCount);
		KTUserC* itemsArray = leaderboard->itemsArray;
		auto rankLayer = RankLayer::getInstance();
		auto view = rankLayer->getFriendScView();
		auto vSize = view->getContentSize();
		float dis = 0.17f * vSize.height;
		float innerHeight = itemCount *dis;
		auto inSize = view->getInnerContainer()->getContentSize();
		if(innerHeight < inSize.height) innerHeight = inSize.height;
		view->setInnerContainerSize(Size(vSize.width,innerHeight));
		float offY = 1.0f * innerHeight;
		float offX = 0.5f * vSize.width;
		if(itemCount >= 100) itemCount = 100;
		for(int i=0;i<itemCount;i++)
		{
			KTUserC user = itemsArray[i];
			log("headerUrl:%s nickname:%s gender:%d score:%s rank:%lld gameUserId:%s needPresentNickname:%d",
					user.headerUrl,user.nickname,user.gender,user.score,user.rank,user.gameUserId,user.needPresentNickname);
			Node* rankNode =  GameData::getInstance()->getFriendsRankItems()[i];
			auto cup = (Sprite*)seekNodeByName(rankNode,"cup");
			auto headImg = (Sprite*)seekNodeByName(rankNode,"headImg");
			auto rankNumLab = (Text*)seekNodeByName(rankNode,"rankNumLab");
			auto nickNameLab = (Text*)seekNodeByName(rankNode,"nickNameLab");
			auto levelLab = (Text*)seekNodeByName(rankNode,"levelLab");
			auto starNumLab = (Text*)seekNodeByName(rankNode,"starNumLab");
			auto scoreLab = (Text*)seekNodeByName(rankNode,"scoreLab");
			auto totaleLab = (Text*)seekNodeByName(rankNode,"totaleLab");
			auto bg = (Sprite*)seekNodeByName(rankNode,"bg");
			auto size = bg->getContentSize();
            int level = GameData::getInstance()->getLevel();
            bool quaryTotalScore = GameData::getInstance()->getQuaryTotalScore();
            const char* headStr = "femaleHead.png";
            if(user.gender == 1)
            {
            	headStr = "maleHead.png";
            }
            auto headFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(headStr);
            headImg->setSpriteFrame(headFrame);

            if(quaryTotalScore)
            {
            	levelLab->setVisible(false);
            	totaleLab->setString("关卡总分");
            }
            else
            {
            	const char* levelStr = String::createWithFormat("第%d关",level)->getCString();
            	levelLab->setString(levelStr);
            	levelLab->setVisible(true);
            	totaleLab->setString("分数");
            }
			const char* str = "";
			if(i<3)
			{
				str = String::createWithFormat("friends_cup%d.png",i+1)->getCString();
				auto newFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
				cup->setSpriteFrame(newFrame);
				rankNumLab->setVisible(false);
				cup->setVisible(true);
			}
			else
			{
				str = String::createWithFormat("%d",i+1)->getCString();
				rankNumLab->setString(str);
				rankNumLab->setVisible(true);
				cup->setVisible(false);
			}
			scoreLab->setString(user.score);
			nickNameLab->setString(user.nickname);
			auto pos = Vec2(offX,offY - size.height/2);
			rankNode->setPosition(pos);
			view->addChild(rankNode);
			offY -= dis;
		}
	}
	else
	{
		log("isError");
	   //操作失败
	}

}

static void globalLeaderboardCallback(bool isSuccess,const char *leaderboardId ,KTLeaderboardPaginatorC *leaderboard,KTErrorC *error)
{
    if (isSuccess)
    {
    	log("isSuccess");
         //操作成功
    	int itemCount = leaderboard->itemCount;
    	log("itemCount:%d",itemCount);
    	KTUserC* itemsArray = leaderboard->itemsArray;
        auto rankLayer = RankLayer::getInstance();
        auto view = rankLayer->getPlayerScView();
        auto vSize = view->getContentSize();
        float dis = 0.17f * vSize.height;
		float innerHeight = itemCount *dis;
		auto inSize = view->getInnerContainer()->getContentSize();
		if(innerHeight < inSize.height) innerHeight = inSize.height;
        view->setInnerContainerSize(Size(vSize.width,innerHeight));
        float offY = 1.0f * innerHeight;
        float offX = 0.5f * vSize.width;
        if(itemCount >= 100) itemCount = 100;
    	for(int i=0;i<itemCount;i++)
    	{
    		KTUserC user = itemsArray[i];
    		log("headerUrl:%s nickname:%s gender:%d score:%s rank:%lld gameUserId:%s needPresentNickname:%d",
    				user.headerUrl,user.nickname,user.gender,user.score,user.rank,user.gameUserId,user.needPresentNickname);
            //Node* rankNode =  CSLoader::createNode("RankItem.csb");
    		Node* rankNode =  GameData::getInstance()->getGlobalRankItems()[i];
            auto cup = (Sprite*)seekNodeByName(rankNode,"cup");
            auto headImg = (Sprite*)seekNodeByName(rankNode,"headImg");
            auto rankNumLab = (Text*)seekNodeByName(rankNode,"rankNumLab");
            auto nickNameLab = (Text*)seekNodeByName(rankNode,"nickNameLab");
            auto levelLab = (Text*)seekNodeByName(rankNode,"levelLab");
            auto starNumLab = (Text*)seekNodeByName(rankNode,"starNumLab");
            auto scoreLab = (Text*)seekNodeByName(rankNode,"scoreLab");
			auto totaleLab = (Text*)seekNodeByName(rankNode,"totaleLab");
			auto bg = (Sprite*)seekNodeByName(rankNode,"bg");
			auto size = bg->getContentSize();
			int level = GameData::getInstance()->getLevel();
			bool quaryTotalScore = GameData::getInstance()->getQuaryTotalScore();
			const char* headStr = "femaleHead.png";
            if(user.gender == 1)
            {
            	headStr = "maleHead.png";
            }
            auto headFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(headStr);
            headImg->setSpriteFrame(headFrame);

			if(quaryTotalScore)
			{
				levelLab->setVisible(false);
				totaleLab->setString("关卡总分");
			}
			else
			{
				const char* levelStr = String::createWithFormat("第%d关",level)->getCString();
				levelLab->setString(levelStr);
				levelLab->setVisible(true);
				totaleLab->setString("分数");
			}
            const char* str = "";
            if(i<3)
            {
            	str = String::createWithFormat("friends_cup%d.png",i+1)->getCString();
            	auto newFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            	cup->setSpriteFrame(newFrame);
            	rankNumLab->setVisible(false);
            	cup->setVisible(true);
            }
            else
            {
            	str = String::createWithFormat("%d",i+1)->getCString();
            	rankNumLab->setString(str);
            	rankNumLab->setVisible(true);
            	cup->setVisible(false);
            }
        	scoreLab->setString(user.score);
            nickNameLab->setString(user.nickname);
            auto pos = Vec2(offX,offY - size.height/2);
            rankNode->setPosition(pos);
            view->addChild(rankNode);
            offY -= dis;
    	}
    }
    else
    {
    	log("isError");
       //操作失败
    }

}



static void reportScoreCallback(bool isSuccess,const char *leaderboardId,long long score,KTErrorC *error)
{
    if (isSuccess)
    {
        //操作成功，
    	log("reportScoreSuccess");
    } else
    {
        //操作失败。leaderboardId ，score 为请求信息
    	log("reportScoreFailed");
    	activityStatusChanged(true);
    	auto winSize = Director::getInstance()->getWinSize();
    	Toast::toast("上传分数失败,点击小水球注册账号,可以冲击排行榜哦",Vec2(winSize.width*0.5f,winSize.height*0.07f),5);
//    	KTAccountManagerC::showLoginView(false, showLoginViewCallback);
    }
}


//创建回调
 void viewDidAppearCallback()
 {
	log("viewDidAppearCallback");
    Director::getInstance()->pause();
 }

 //创建回调
 void onKTDisappear()
 {
	 log("onKTDisappear");
	 Director::getInstance()->resume();
 }

void AndroidPlatform::platform(CMD_ID state,int infoNum,string infoStr)
{
    switch (state)
    {
        case SHOW_KTPLAY:
            {
            	KTPlayC::show();
            }
            break;
        case DISPATCH_REWARD:
            {
            	KTPlayC::setDidDispatchRewardsCallback(dispatchRewards);
            }
            break;
        case STATUS_CHANGED:
        	{
        		KTPlayC::setActivityStatusChangedCallback(activityStatusChanged);
        	}
        	break;
        case SHOW_INTERSTITIAL_NOTIFICATION:
        	{
        		KTPlayC::showInterstitialNotification();
        	}
        	break;
        case AVAILABILITY_CHANGED:
        	{
        		KTPlayC::setAvailabilityChangedCallback(availabilityChangedCallback);
        	}
    		break;
        case DEEP_LINNK:
            {
                KTPlayC::setDeepLinkCallback(deepLinkCallback);
            }
        	break;
        case LOGINWITH_GAMEUSE:
        	{
        		KTAccountManagerC::loginWithGameUser("skillart", loginWithGameUserCallback);
        	}
        	break;
        case SET_NiCKNAME:
        	{
        		KTAccountManagerC::setNickName("opop", setNickNameCallBack);
        	}
        	break;
        case SHOW_LOGINVIEW:
        	{
        		KTAccountManagerC::showLoginView(false, showLoginViewCallback);
        	}
        	break;
        case LOGIN_STATUSCHANGED:
        	{
        		KTAccountManagerC::setLoginStatusChangedCallback(loginStatusChangedCallback);
        	}
        	break;
        case IS_LOGGEDIN:
        	{
        		bool isLoggedIn = KTAccountManagerC::isLoggedIn();
        	}
        	break;
        case USE_RPROFILE:
        	KTAccountManagerC::userProfile("1234" ,userProfileCallback);
        	break;
        case CURRENT_ACCOUNT:
        	{
        		KTUserC *userC = KTAccountManagerC::currentAccount();
        	}
        	break;
        case LOGOUT:
        	KTAccountManagerC::logout();
        	break;
        case SHOW_FRIEND_VIEW:
        	break;
        case SEND_FRIEND_REQUEST:
        	break;
        case FRIEND_LIST:
        	break;
		case FRIEND_LEADER_BOARD:
			{
				const char* boardID = String::createWithFormat("%d",infoNum)->getCString();
				KTLeaderboardC::friendsLeaderboard(boardID, 0, 20, friendLeaderboardCallback);
			}
			break;
		case GLOBAL_LEADER_BOARD:
			{
				const char* boardID = String::createWithFormat("%d",infoNum)->getCString();
				KTLeaderboardC::globalLeaderboard(boardID, 0, 100, globalLeaderboardCallback);
			}
			break;
		case REPORT_SCORE:
			{
				const char* boardID = String::createWithFormat("%d",infoNum)->getCString();
				long long score = atoll(infoStr.c_str());
                log("score:%lld %s",score,boardID);
				KTLeaderboardC::reportScore(score,boardID,reportScoreCallback);
			}
            break;
        case KTPLAY_DIDAPPEAR:
        	{
        		KTPlayC::setViewDidAppearCallback(viewDidAppearCallback);
        	}
        	break;
        case KTPLAY_DISAPPEAR:
        	{
        		KTPlayC::setViewDidDisappearCallback(onKTDisappear);
        	}
        	break;
        case KTPLAY_DISMISS:
        	{
        		KTPlayC::dismiss();
        	}
        	break;
        case IS_KTSHOWING:
        	{
        		bool isKTShowing = KTPlayC::isShowing();
        	}
        	break;
        default:
            break;
    }
    
//    JniMethodInfo minfo;
//    jobject jobj;
//    if (JniHelper::getStaticMethodInfo(minfo, CLASSNAME,
//                                       "getInstance", "()Ljava/lang/Object;"))
//    {
//        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//        if (JniHelper::getMethodInfo(minfo, CLASS_NAME, "jni_called", "(II)V"))
//        {
//            if (jobj)
//            {
//                minfo.env->CallVoidMethod(jobj, minfo.methodID, state, 0);
//            }
//        }
//    }
}
string AndroidPlatform::platform(string jsonStr)
{
    JniMethodInfo minfo;
    jobject jobj;
    string str = "";
    const char* toCharStr = jsonStr.c_str();
    if (JniHelper::getStaticMethodInfo(minfo, CLASS_NAME,
                                       "getInstance", "()Ljava/lang/Object;"))
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (JniHelper::getMethodInfo(minfo, CLASS_NAME, "jni_called", "(Ljava/lang/String;)Ljava/lang/String;"))
        {
            if (jobj)
            {
                jstring jstr = minfo.env->NewStringUTF(toCharStr);
                jstring jrstr = (jstring)minfo.env->CallObjectMethod(jobj, minfo.methodID, jstr);
                str=JniHelper::jstring2string(jrstr);
                minfo.env->DeleteLocalRef(jstr);
            }
        }
    }
    return str;
}


bool AndroidPlatform::isLogin()
{
   return KTAccountManagerC::isLoggedIn();
}

extern "C"
{
    void Java_com_fuatnow_game_sealink_AppActivity_AndroidCallBack(JNIEnv *env, jobject thiz, jstring jsonStr)
    {
        const char *str = env->GetStringUTFChars(jsonStr, NULL);

        log("jsonStr:%s",str);

        Document doc;///<创建一个Document对象rapidJson的相关操作都在Document类中
		doc.Parse<0>(str);///<通过Parse方法将Json数据解析出来
		if(doc.HasParseError())
		{
			log("GetParseError%s\n",doc.GetParseError());
		}

        rapidjson::Value& cmdValue=doc["cmdId"];
        const char* cmdIdStr = doc["cmdId"].GetString();
        int cmdId = atoi(cmdIdStr);
        log("cmdId:%d",cmdId);
        switch(cmdId)
        {
        case 0:
        	break;
        case CMD_ID_PAY:
        	ShopLayer::getInstance()->payInfo(str);
        	break;
        case CMD_ID_DOWNAPP:
            HomeLayer::getInstance()->downNewAppInfo(str);
            break;
        case CMD_ID_EXITGAME:
            {
                umeng::MobClickCpp::end();
                Director::getInstance()->end();
            }
            break;
        default:
        	break;
        }
        env->ReleaseStringUTFChars(jsonStr,str);
    }
}


#endif
