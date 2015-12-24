//
//  BuyDiamondDialog.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#ifndef __SeaLink__BuyDiamondDialog__
#define __SeaLink__BuyDiamondDialog__


#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class BuyDiamondDialog: public BasicLayer
{
public:
    BuyDiamondDialog(){};
    ~BuyDiamondDialog(){};
    static Scene* scene();
    virtual bool init();
    void initData();
    void initView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    void img_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(BuyDiamondDialog);
public:
    std::vector<int>diamondVec;
    std::vector<int>monyVec;
};

#endif /* defined(__SeaLink__BuyDiamondDialog__) */
