//
//  ConverDialog.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#ifndef __SeaLink__ConverDialog__
#define __SeaLink__ConverDialog__

#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class ConverDialog: public BasicLayer
{
public:
    ConverDialog(){};
    ~ConverDialog(){};
    static Scene* scene();
    virtual bool init();
    void initData();
    void initView();
    void close_callBack(Ref *pSender, Widget::TouchEventType type);
    void confirm_callBack(Ref *pSender, Widget::TouchEventType type);
    void textFieldEvent(Ref *pSender, TextField::EventType type);
    CREATE_FUNC(ConverDialog);
private:
    Text* msgLab;
    Node* bg;
    TextField* inputText;
};

#endif /* defined(__SeaLink__ConverDialog__) */
