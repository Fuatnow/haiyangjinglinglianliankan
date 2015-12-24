//
//  HomeLayer.h
//  Link
//
//  Created by Fuatnow on 15-3-27.
//
//

#ifndef __Link__HomeLayer__
#define __Link__HomeLayer__

#include "Tools.h"
#include "GameLayer.h"
#include "BasicLayer.h"
#include "BackgroundLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class HomeLayer: public BasicLayer
{
public:
    HomeLayer(){};
    ~HomeLayer(){};
    static Scene* scene();
    static HomeLayer* getInstance();
    virtual bool init();
    void initData();
    void initView();
    void start_callBack(Ref *pSender, Widget::TouchEventType type);
    void sound_callBack(Ref *pSender, Widget::TouchEventType type);
    void music_callBack(Ref *pSender, Widget::TouchEventType type);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void downNewAppInfo(std::string jsonStr);
    void testHaveNeedDownNewApp();
    void foreUpdateApp();//强制更新app
    void pareseJsonTest();
    CREATE_FUNC(HomeLayer);
public:
    Button* startItem;
    Button* soundItem_off;
    Button* soundItem;
    Button* musicItem_off;
    Button* musicItem;
    //是否需要更新app
    bool haveNeedDownNewApp;
    //是否需要强制更新app
    bool isNeedForeUpdate;
    //是否点击过暂不更新
    bool hadClickNotUpdate;
};

#endif /* defined(__Link__HomeLayer__) */
