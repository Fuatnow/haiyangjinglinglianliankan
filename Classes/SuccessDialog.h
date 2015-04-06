//
//  SuccessDialog.h
//  Link
//
//  Created by Fuatnow on 15-3-30.
//
//

#ifndef __Link__SuccessDialog__
#define __Link__SuccessDialog__

#include "BasicLayer.h"
#include "Define.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Tools.h"
#include "Define.h"
using namespace ui;
using namespace std;

class SuccessDialog : public Node
{
public:
    SuccessDialog(){};
    ~SuccessDialog(){};
    virtual bool init();
    void initData();
    void star_callBack(Ref* pSender);
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(SuccessDialog);
};

#endif /* defined(__Link__SuccessDialog__) */
