//
//  TransitionMoveRL.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#include "TransitionMoveRL.h"
TransitionMoveRL::TransitionMoveRL()
{
}
TransitionMoveRL::~TransitionMoveRL()
{
}

TransitionMoveRL* TransitionMoveRL::create(float t, Scene* scene)
{
    TransitionMoveRL* newScene = new (std::nothrow) TransitionMoveRL();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveRL::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(-s.width,0);
}

void TransitionMoveRL::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();
    
    Size s = Director::getInstance()->getWinSize();
    ActionInterval *ain = MoveTo::create(_duration, Vec2(0,0));
    ActionInterval *aout = MoveTo::create(_duration, Vec2(s.width,0));
    
    _inScene->runAction
    (
     Sequence::create
     (
      this->easeActionWithAction(ain),
      CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
      nullptr
      )
     );
    
    _outScene->runAction
    (
     Sequence::create
     (
      this->easeActionWithAction(aout),
      CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
      nullptr
      )
     );
    
}