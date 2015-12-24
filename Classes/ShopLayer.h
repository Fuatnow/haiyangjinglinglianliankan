//
//  ShopLayer.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#ifndef __SeaLink__ShopLayer__
#define __SeaLink__ShopLayer__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class ShopLayer: public BasicLayer
{
public:
    ShopLayer(){};
    ~ShopLayer(){};
    static Scene* scene();
    static ShopLayer* getInstance();
    virtual bool init();
    void initData();
    void initView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    void help_callBack(Ref *pSender, Widget::TouchEventType type);
    void add_callBack(Ref *pSender, Widget::TouchEventType type);
    void con_callBack(Ref *pSender, Widget::TouchEventType type);
    void updateCoinAndDiamondNum();
    void payInfo(std::string jsonStr);
    void giveDiamond(int payId);
    void buyPropSuccess();
    CREATE_FUNC(ShopLayer);
private:
    std::vector<int> diamondVec;
    std::vector<std::string> diamondInfo;
    TextBMFont* diamonLab;
    TextBMFont* coinLab;
    int payPropId;
};


#endif /* defined(__SeaLink__ShopLayer__) */
