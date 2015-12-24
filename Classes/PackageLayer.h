//
//  PackageLayer.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-12.
//
//

#ifndef __SeaLink__PackageLayer__
#define __SeaLink__PackageLayer__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class PackageLayer: public BasicLayer
{
public:
    PackageLayer(){};
    ~PackageLayer(){};
    static Scene* scene();
    virtual bool init();
    void initData();
    void initView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(PackageLayer);
};


#endif /* defined(__SeaLink__PackageLayer__) */
