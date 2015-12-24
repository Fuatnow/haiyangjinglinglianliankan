//
//  Toast.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#include "Toast.h"

static Toast* instance = NULL;

Toast::~Toast()
{
    auto scheduler =  Director::getInstance()->getScheduler();
    scheduler->unschedule(scheduler1, this);
    
    auto listener = Director::getInstance()->getEventDispatcher();
    listener->removeEventListener(listener1);
    
    instance = NULL;
}


void Toast::toast(std::string msg,Vec2 pos,float time)
{
    Toast::getInstance()->show(msg,pos,time);
}

Toast* Toast::getInstance()
{
    if(instance == NULL)
    {
        instance = Toast::create();
        auto notiNode = Director::getInstance()->getNotificationNode();
        if(notiNode == NULL)
        {
            Director::getInstance()->setNotificationNode(instance);
        }
        else
        {
            notiNode->addChild(instance);
        }
    }
    
    return instance;
}

bool Toast::init()
{
    Node::init();
    
    log("Toast init");
    
    setVisible(true);
    
    auto rootNode =  CSLoader::createNode("Toast.csb");
    this->addChild(rootNode);
    bg = (Button*)seekNodeByName(rootNode, "bg");
    msgLab = (Text*)seekNodeByName(rootNode, "msgLab");
    
    _action = NULL;
    
    listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
    listener1->setSwallowTouches(false);//设置是否想下传递触摸
    listener1->onTouchBegan = [=](Touch* touch, Event* event)
    {
        Vec2 location = bg->convertToNodeSpace(touch->getLocation());
        Size s = bg->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        bool isInTouch = rect.containsPoint(location);
        if(isInTouch)
        {
            btn_callBack();
        }
        return isInTouch;
    };

    auto listener = Director::getInstance()->getEventDispatcher();
    listener->addEventListenerWithFixedPriority(listener1, -100);
    
    auto scheduler =  Director::getInstance()->getScheduler();
    scheduler1 = schedule_selector(Toast::update);
    scheduler->schedule(scheduler1,this, 0, CC_REPEAT_FOREVER, 0, false);
    
    return true;
}

void Toast::update(float dt)
{
    if(_action)
    {
        _action->step(dt);
    }
    
//    if(instance)
//    {
//        auto notiNode = Director::getInstance()->getNotificationNode();
//        log("err:%d",instance->getReferenceCount());
//    }
}

void Toast::show(std::string msg,Vec2 pos,float time)
{
    log("uuaaui:  %s",msg.c_str());
    
    setVisible(true);
    
    msgLab->setString(msg);
    auto size = msgLab->getContentSize();
    auto bgSize = bg->getContentSize();
    bgSize.width = size.width * 1.2f;
    bg->setContentSize(bgSize);
    msgLab->setPosition(bgSize/2);
    
    auto winSize = Director::getInstance()->getWinSize();
    if(pos.equals(Vec2(0, 0)))
    {
        pos = Vec2(winSize.width/2,winSize.height*0.15f);
    }
    setPosition(pos);
    
    auto fadeOut = FadeOut::create(time);
    auto callBack = CallFuncN::create(CC_CALLBACK_1(Toast::action_callBack,this));
    _action = Sequence::create(fadeOut,callBack,NULL);
    runAction(_action);
}

void Toast::onExit()
{
    log("Toast::onExit");
    Node::onExit();
}

void Toast::btn_callBack()
{
    action_callBack(NULL);
}

void Toast::action_callBack(cocos2d::Ref *pSender)
{
    setOpacity(255);
    setVisible(false);
    _action = NULL;
}