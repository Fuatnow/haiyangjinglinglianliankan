//
//  TransitionMoveLR.h
//  Link
//
//  Created by Fuatnow on 15-4-4.
//
//

#ifndef __Link__TransitionMoveLR__
#define __Link__TransitionMoveLR__

#include "cocos2d.h"
USING_NS_CC;

class TransitionMoveLR : public TransitionMoveInR
{
public:
    static TransitionMoveLR* create(float t, Scene* scene);
    //
    // Overrides
    //
    virtual void onEnter() override;
CC_CONSTRUCTOR_ACCESS:
    TransitionMoveLR();
    virtual ~TransitionMoveLR();
    
protected:
    virtual void initScenes();
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(TransitionMoveLR);
};

#endif /* defined(__Link__TransitionMoveLR__) */
