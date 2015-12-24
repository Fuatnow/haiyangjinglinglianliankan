//
//  IntroduceLayer.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#ifndef __SeaLink__IntroduceLayer__
#define __SeaLink__IntroduceLayer__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class IntroduceLayer: public BasicLayer
{
public:
    IntroduceLayer(){};
    ~IntroduceLayer(){};
    static Scene* scene();
    virtual bool init();
    void initData();
    void initView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(IntroduceLayer);
};

#endif /* defined(__SeaLink__IntroduceLayer__) */
