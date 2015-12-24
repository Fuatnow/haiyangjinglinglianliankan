//
//  TransitionMoveRL.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-17.
//
//

#ifndef __SeaLink__TransitionMoveRL__
#define __SeaLink__TransitionMoveRL__

#include "cocos2d.h"
USING_NS_CC;

class TransitionMoveRL : public TransitionMoveInR
{
public:
    static TransitionMoveRL* create(float t, Scene* scene);
    //
    // Overrides
    //
    virtual void onEnter() override;
CC_CONSTRUCTOR_ACCESS:
    TransitionMoveRL();
    virtual ~TransitionMoveRL();
    
protected:
    virtual void initScenes();
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(TransitionMoveRL);
};

#endif /* defined(__SeaLink__TransitionMoveRL__) */
