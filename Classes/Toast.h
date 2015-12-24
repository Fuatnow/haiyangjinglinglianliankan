//
//  Toast.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#ifndef __SeaLink__Toast__
#define __SeaLink__Toast__
#include "Tools.h"
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class Toast : public Node
{
public:
    Toast(){};
    ~Toast();
    static Toast* getInstance();
    static void toast(std::string msg, Vec2 pos = Vec2(0,0) , float time = 3.0f);
    virtual bool init();
    virtual void update(float dt);
    virtual void onExit();
private:
    CREATE_FUNC(Toast);
    void show(std::string msg,Vec2 pos,float time);
    void btn_callBack();
    void action_callBack(Ref *pSender);
private:
    Button* bg;
    Text* msgLab;
    ActionInterval* _action;
    EventListenerTouchOneByOne* listener1;
    SEL_SCHEDULE scheduler1;
};

#endif /* defined(__SeaLink__Toast__) */
