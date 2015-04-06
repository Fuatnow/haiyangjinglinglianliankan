//
//  PauseDialog.h
//  Link
//
//  Created by Fuatnow on 15-4-1.
//
//

#ifndef __Link__PauseDialog__
#define __Link__PauseDialog__

#include "BasicLayer.h"
#include "Define.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Tools.h"
#include "Define.h"
using namespace ui;
using namespace std;

class PauseDialog : public Node
{
public:
    PauseDialog(){};
    ~PauseDialog(){};
    virtual bool init();
    void initData();
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(PauseDialog);
public:
    Button* musicBtnOn;
    Button* musicBtnOff;
    Button* soundBtnOn;
    Button* soundBtnOff;
};

#endif /* defined(__Link__PauseDialog__) */
