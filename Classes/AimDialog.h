//
//  AimDialog.h
//  Link
//
//  Created by Fuatnow on 15-3-30.
//
//

#ifndef __Link__AimDialog__
#define __Link__AimDialog__

#include "BasicLayer.h"
#include "Define.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Tools.h"
#include "Define.h"
#include <iostream>
using namespace ui;
using namespace std;

class AimDialog : public Node
{
public:
    AimDialog(){};
    ~AimDialog(){};
    virtual bool init();
    void initData();
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(AimDialog);
};

#endif /* defined(__Link__AimDialog__) */
