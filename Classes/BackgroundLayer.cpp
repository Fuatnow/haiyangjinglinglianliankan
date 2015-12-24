#include "BackgroundLayer.h"
#include "Audio.h"
#include "GameData.h"
#include "Resounce.h"
#include "ActiveGrass.h"
#include "Tools.h"
bool BackgroundLayer::init()
{
	BasicLayer::init();
	initView();
	return true;
}

void BackgroundLayer::initView()
{
    Node* node = CSLoader::createNode("background.csb");
    this->addChild(node);
    auto ligth = node->getChildByName("light");
    auto action1 = FadeOut::create(2.0f);
    auto action2 = FadeIn::create(2.0f);
    auto action3 = DelayTime::create(0.2f);
    ligth->runAction(RepeatForever::create(Sequence::create(action1,action2,action3,NULL)));
    
    for(int i=1;i<=3;i++)
    {
        const char* sandStr = String::createWithFormat("sand_%02d",i)->getCString();
        auto sand = (Node*)seekNodeByName(node, sandStr);
        sand->setZOrder(2);
    }
    
    auto grass = ActiveGrass::create("grass.png");
    grass->setAnchorPoint(Vec2(0,0));
    addChild(grass,1);
    
    this->setScale(ppuScale);
}

