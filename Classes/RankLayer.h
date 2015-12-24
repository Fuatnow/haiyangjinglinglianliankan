//
//  RankLayer.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#ifndef __SeaLink__RankLayer__
#define __SeaLink__RankLayer__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class RankLayer: public BasicLayer
{
public:
    RankLayer(){};
    ~RankLayer();
    static Scene* scene();
    static RankLayer* getInstance();
    virtual bool init();
    virtual void onExit();
    void initData();
    void initView();
    void loadLeaderboard(int boardID);
    void initPlayerView();
    void initFriendView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    void type_callBack(Ref *pSender, Widget::TouchEventType type);
    void appear();
    CREATE_FUNC(RankLayer);
public:
    Button* playerBtn;
    Button* playerSelBtn;
    Button* friendBtn;
    Button* friendSelBtn;
    Node* bg;
    CC_SYNTHESIZE(ui::ScrollView* , playerScView , PlayerScView);
    CC_SYNTHESIZE(ui::ScrollView* , friendScView , FriendScView);
};


#endif /* defined(__SeaLink__RankLayer__) */
