//
//  LoadingLayer.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-4-16.
//
//

#include "LoadingLayer.h"
#include "HomeLayer.h"
#include "Audio.h"
#include "Define.h"
#include "GlobalNode.h"
#include "Platform.h"
#include "GameData.h"
#define INIT_OBJECT_POOL 202
Scene* LoadingLayer::scene()
{
    auto sc = Scene::create();
    auto layer =  LoadingLayer::create();
    sc->addChild(layer);
    return sc;
}

bool LoadingLayer::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void LoadingLayer::initData()
{
    
    loadTextures.push_back("background.png");
    loadTextures.push_back("cell.png");
    loadTextures.push_back("itemLayer.png");
    loadTextures.push_back("levelLayer.png");
    loadTextures.push_back("panel.png");
    loadTextures.push_back("shopLayer.png");
    loadTextures.push_back("water.png");
//    loadTextures.push_back("");
//    loadTextures.push_back("");
//    loadTextures.push_back("");
//    loadTextures.push_back("");
//    loadTextures.push_back("");
//    loadTextures.push_back("");
    
    
    loadTextures.push_back("levelBg_0.jpg");
    loadTextures.push_back("levelBg_1.jpg");
    loadTextures.push_back("levelBg_2.jpg");
    loadTextures.push_back("levelBg_3.jpg");
    loadTextures.push_back("levelBg_4.jpg");
    loadTextures.push_back("levelBg_5.jpg");
    loadTextures.push_back("scroBg.jpg");
    loadTextures.push_back("logo.jpg");
    
    hasLoadNums = 0;
    needLoadNum = loadTextures.size() + INIT_OBJECT_POOL;
}

void LoadingLayer::initView()
{
    auto rootNode =  CSLoader::createNode("LoadingLayer.csb");
    this->addChild(rootNode);

    loadingBar = (LoadingBar*)seekNodeByName(rootNode, "loadingBar");
    particle = (ParticleSystem*)seekNodeByName(rootNode, "particle");
    loadTip = (Text*)seekNodeByName(rootNode, "loadTip");
    loadTip->setString("超神加载中...(此过程不会产生任何流量)");
    loadProgress = (TextBMFont*)seekNodeByName(rootNode, "loadProgress");
    auto loadBg = (Sprite*)seekNodeByName(rootNode, "barBg");
    loadSize = loadBg->getContentSize();
    
    auto texCa = Director::getInstance()->getTextureCache();
    for(int i=0;i<loadTextures.size();i++)
    {
        texCa->addImageAsync(loadTextures[i],CC_CALLBACK_1(LoadingLayer::load_callBack,this));
    }
}

void LoadingLayer::loadAllPlists()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cell.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("itemLayer.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("levelLayer.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("panel.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shopLayer.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background.plist");
}

void LoadingLayer::update(float dt)
{
    loadObject_callBack(NULL);
}


void LoadingLayer::load_callBack(cocos2d::Texture2D* texture)
{
    hasLoadNums++;
    log("hasLoadNums:%d",hasLoadNums);
    auto size = loadingBar->getContentSize();
    float per = hasLoadNums/needLoadNum;
    const char* perStr = String::createWithFormat("%.f%%",per*100)->getCString();
    loadProgress->setString(perStr);
    loadingBar->setPercent(per * 100);
    particle->setPositionX(loadSize.width*per);
    if(hasLoadNums == loadTextures.size())
    {
        /*
        float loadOneObjTime = rand_0_1()*0.02f + 0.025f;
        //初始化INIT_OBJECT_POOL个对象
        for(int i=0;i<INIT_OBJECT_POOL;i++)
        {
            float delayTime = i * loadOneObjTime;
            auto callBack = CallFuncN::create(CC_CALLBACK_1(LoadingLayer::loadObject_callBack,this));
            auto loadAction = Sequence::create(DelayTime::create(delayTime),callBack, NULL);
            this->runAction(loadAction);
        }
        */
        
        scheduleUpdate();
    }
    else if(abs(needLoadNum - hasLoadNums) <0.1f)
    {
//        log("%f %d",needLoadNum,hasLoadNums);
        unscheduleUpdate();
        loadTip->setString("加载完成");
        loadAllPlists();
        
        auto sc = HomeLayer::scene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sc));
    }
}

void LoadingLayer::loadObject_callBack(Node* pSender)
{
    auto rankNode =  CSLoader::createNode("RankItem.csb");
    rankNode->retain();
    if(hasLoadNums - loadTextures.size() <= INIT_OBJECT_POOL/2)
    {
        GameData::getInstance()->getGlobalRankItems().push_back(rankNode);
    }
    else
    {
        GameData::getInstance()->getFriendsRankItems().push_back(rankNode);
    }
    load_callBack(NULL);
//    hasLoadNums++;
//    GameData::getInstance()->getGlobalRankItems().push_back(rankNode1);
//    auto rankNode2 =  CSLoader::createNode("RankItem.csb");
//    rankNode2->retain();
//    hasLoadNums++;
//    GameData::getInstance()->getFriendsRankItems().push_back(rankNode2);
}
