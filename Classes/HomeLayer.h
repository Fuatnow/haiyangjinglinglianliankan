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
    virtual bool init();
    void initData();
    void initView();
    void start_callBack(Ref *pSender, Widget::TouchEventType type);
    void sound_callBack(Ref *pSender, Widget::TouchEventType type);
    void music_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(HomeLayer);
public:
    Button* startItem;
    Button* soundItem_off;
    Button* soundItem;
    Button* musicItem_off;
    Button* musicItem;
};

#endif /* defined(__Link__HomeLayer__) */
