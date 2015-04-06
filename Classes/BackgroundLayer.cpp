#include "BackgroundLayer.h"
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
}

