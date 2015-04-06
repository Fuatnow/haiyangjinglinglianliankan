//
//  TransitionMoveLR.cpp
//  Link
//
//  Created by Fuatnow on 15-4-4.
//
//

#include "TransitionMoveLR.h"


TransitionMoveLR::TransitionMoveLR()
{
}
TransitionMoveLR::~TransitionMoveLR()
{
}

TransitionMoveLR* TransitionMoveLR::create(float t, Scene* scene)
{
    TransitionMoveLR* newScene = new (std::nothrow) TransitionMoveLR();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveLR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(s.width,0);
}

void TransitionMoveLR::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();
    
    Size s = Director::getInstance()->getWinSize();
    ActionInterval *ain = MoveTo::create(_duration, Vec2(0,0));
    ActionInterval *aout = MoveTo::create(_duration, Vec2(-s.width,0));
    
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