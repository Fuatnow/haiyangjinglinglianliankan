//
//  GlobalNode.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-13.
//
//

#include "GlobalNode.h"
#include "Platform.h"

GlobalNode::~GlobalNode()
{
    auto scheduler =  Director::getInstance()->getScheduler();
    scheduler->unschedule(scheduler1, this);
    
    auto listener = Director::getInstance()->getEventDispatcher();
    listener->removeEventListener(listener1);
}

bool GlobalNode::init()
{
    Node::init();
    initData();
    initView();
    return true;
}

void GlobalNode::initData()
{
    isInTouch = false;
    isBtnMove = false;
    fallow_action = NULL;
    corner_action = NULL;
    notice_action = NULL;
    winWidth = Director::getInstance()->getWinSize().width;
    winHeight = Director::getInstance()->getWinSize().height;
    stayRect = Rect(0.15f*winWidth,0.2f*winHeight,0.7f*winWidth,0.6f*winHeight);
}

void GlobalNode::initView()
{
    btn = Sprite::createWithSpriteFrameName("globalBoard.png");
    btn->setPosition(Vec2(0.2f*winWidth,0.8f*winHeight));
    addChild(btn);
    
    auto bSize = btn->getContentSize();
    noticeSp = Sprite::createWithSpriteFrameName("kryptanium_frineds_red_point.png");
    noticeSp->setPosition(bSize/2);
    btn->addChild(noticeSp);
    
    auto bs = btn->getContentSize();
    auto posLU = Vec2(0,winHeight) + Vec2(bs.width/2,-bs.height/2);
    auto posLB = Vec2(0,0) + Vec2(bs.width/2,bs.height/2);
    auto posRU = Vec2(winWidth,winHeight) + Vec2(-bs.width/2,-bs.height/2);
    auto posRB = Vec2(winWidth,0) + Vec2(-bs.width/2,bs.height/2);
    cornerVec.push_back(posLU);
    cornerVec.push_back(posLB);
    cornerVec.push_back(posRU);
    cornerVec.push_back(posRB);
    
    listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
    listener1->setSwallowTouches(true);//设置是否想下传递触摸
    listener1->onTouchBegan = [=](Touch* touch, Event* event)
    {
    	isBtnMove = true;
    	prePos = btn->getPosition();
        Vec2 location = btn->convertToNodeSpace(touch->getLocation());
        Size s = btn->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        isInTouch = rect.containsPoint(location);
        if(isInTouch)
        {
            //log("isInTouchBegan%d",isInTouch);
        }
        return isInTouch;
    };
    
    //拖动精灵移动
    listener1->onTouchMoved = [&](Touch* touch, Event* event)
    {
          btn->setPosition(btn->getPosition() + touch->getDelta());
//        log("%f %f",touch->getDelta().x,touch->getDelta().y);
//        auto startPos = touch->getStartLocation();
//        auto curPos = touch->getLocation();
//        if(startPos.distance(curPos) < 0.1f && fallow_action) return;
//        auto callBack = CallFuncN::create(CC_CALLBACK_1(GlobalNode::fallowCallBack,this));
//        fallow_action = Sequence::create(MoveTo::create(0.2f, btn->getPosition() + curPos - startPos),callBack,NULL);
//        btn->runAction(fallow_action);
    };
    
    listener1->onTouchEnded = [&](Touch* touch, Event* event)
    {
        isBtnMove = false;
    	Vec2 curPos = btn->getPosition();
    	if(curPos.distance(prePos) > 5)
    	{
    		isBtnMove = true;
    	}

    	if(stayRect.containsPoint(curPos) == false)
        {
            float minDis = winWidth;
            int cornerIndex = 0;
            for(int i=0;i<4;i++)
            {
                Vec2 pos = cornerVec[i];
                float dis = curPos.distance(pos);
                if(dis < minDis)
                {
                    minDis = dis;
                    cornerIndex = i;
                }
            }

            float moevTime = minDis / winWidth;
            auto callBack = CallFuncN::create(CC_CALLBACK_1(GlobalNode::cornerCallBack,this));
            corner_action = Sequence::create(MoveTo::create(moevTime, cornerVec[cornerIndex]),callBack,NULL);
            btn->runAction(corner_action);
        }
    };
    auto listener = Director::getInstance()->getEventDispatcher();
    listener->addEventListenerWithFixedPriority(listener1, -100);
    
    scheduler1 = schedule_selector(GlobalNode::update);
    auto scheduler =  Director::getInstance()->getScheduler();
    scheduler->schedule(scheduler1,this, 0, CC_REPEAT_FOREVER, 0, false);
}

void GlobalNode::update(float dt)
{
     if(corner_action)
     {
    	 corner_action->step(dt);
     }
    
     if(fallow_action)
     {
        fallow_action->step(dt);
     }
    
     if(notice_action)
     {
         notice_action->step(dt);
     }
    
    if(isBtnMove == false && isInTouch)
    {
        btn_callBack();
    }
}

void GlobalNode::btn_callBack()
{
    log("SHOW_KTPLAY");
    Platform::getInstance()->platform(SHOW_KTPLAY, 0);
    noticeSp->stopAllActions();
    noticeSp->setVisible(true);
    notice_action = NULL;
    isInTouch = false;
}

void GlobalNode::newNotice()
{
    notice_action = RepeatForever::create(Blink::create(1, 1));
    noticeSp->runAction(notice_action);
}

void GlobalNode::hideNotice()
{
	notice_action = NULL;
    noticeSp->setVisible(true);
}

void GlobalNode::cornerCallBack(Node* pSender)
{
    log("cornerCallBack");
    corner_action = NULL;
}

void GlobalNode::fallowCallBack(Node* pSender)
{
    log("fallowCallBack");
    fallow_action = NULL;
}
