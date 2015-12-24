//
//  BuyTipDialog.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#ifndef __SeaLink__BuyTipDialog__
#define __SeaLink__BuyTipDialog__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class BuyTipDialog: public BasicLayer
{
public:
    BuyTipDialog(){};
    ~BuyTipDialog(){};
    static Scene* scene();
    virtual bool init();
    void initData();
    void initView();
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(BuyTipDialog);
public:
    std::vector<int>diamondVec;
    std::vector<int>monyVec;
};


#endif /* defined(__SeaLink__BuyTipDialog__) */
