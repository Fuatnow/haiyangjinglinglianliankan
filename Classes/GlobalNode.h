//
//  GlobalNode.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#ifndef __SeaLink__GlobalNode__
#define __SeaLink__GlobalNode__
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BasicLayer.h"
using namespace ui;
class GlobalNode : public Node
{
public:
    GlobalNode(){};
    ~GlobalNode();
    bool init();
    void initData();
    void initView();
    void update(float dt);
    CREATE_FUNC(GlobalNode);
    void btn_callBack();
    void cornerCallBack(Node* pSender);
    void fallowCallBack(Node* pSender);
    void newNotice();
    void hideNotice();
private:
    std::vector<Vec2> cornerVec;
    Sprite* btn;
    Sprite* noticeSp;
    float winWidth;
    float winHeight;
    bool isInTouch;
    bool isBtnMove;
    Vec2 prePos;
    //btn 跟随的action
    ActionInterval* fallow_action;
    //btn 靠边的action
    ActionInterval* corner_action;
    //有新消息的action
    ActionInterval* notice_action;
    Rect stayRect;
    
    //全局的
    EventListenerTouchOneByOne* listener1;
    SEL_SCHEDULE scheduler1;
};

#endif /* defined(__SeaLink__GlobalNode__) */
