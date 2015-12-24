//
//  LoadingLayer.h
//  SeaLink
//
//  Created by Fuatnow on 15-4-16.
//
//

#ifndef __SeaLink__LoadingLayer__
#define __SeaLink__LoadingLayer__

#include "Tools.h"
#include "GameLayer.h"
#include "BasicLayer.h"
#include "BackgroundLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;
class LoadingLayer: public BasicLayer
{
public:
    LoadingLayer(){};
    ~LoadingLayer(){};
    static Scene* scene();
    virtual bool init();
    virtual void update(float dt);
    void initData();
    void initView();
    void loadAllPlists();
    void load_callBack(cocos2d::Texture2D* texture);
    void loadObject_callBack(Node* pSender);
    CREATE_FUNC(LoadingLayer);
public:
    vector<string>loadTextures;
    int hasLoadNums;
    LoadingBar* loadingBar;
    ParticleSystem* particle;
    Text* loadTip;
    TextBMFont* loadProgress;
    Size loadSize;
    float needLoadNum;
};

#endif /* defined(__SeaLink__LoadingLayer__) */
