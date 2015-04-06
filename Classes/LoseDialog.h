//
//  LoseDialog.h
//  Link
//
//  Created by Fuatnow on 15-4-1.
//
//

#ifndef __Link__LoseDialog__
#define __Link__LoseDialog__

#include "BasicLayer.h"
#include "Define.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Tools.h"
#include "Define.h"
using namespace ui;
using namespace std;

class LoseDialog : public Node
{
public:
    LoseDialog(){};
    ~LoseDialog(){};
    virtual bool init();
    void initData();
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(LoseDialog);
};

#endif /* defined(__Link__LoseDialog__) */
