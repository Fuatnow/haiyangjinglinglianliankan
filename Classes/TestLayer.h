//
//  TestLayer.h
//  Link
//
//  Created by Fuatnow on 15-4-4.
//
//

#ifndef __Link__TestLayer__
#define __Link__TestLayer__

#include "cocos2d.h"

using namespace cocos2d;

class TestLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initView();
    // implement the "static create()" method manually
    CREATE_FUNC(TestLayer);
    
private:
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::TMXLayer *_background;
};
#endif /* defined(__Link__TestLayer__) */
