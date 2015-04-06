#include "LevelLayer.h"
#include "math.h"
#include "GameData.h"
#include "AimDialog.h"
#include "Audio.h"
#define  HERO_JUMP_HEIGHT 50
Btn_FrameName btnFramNames[CHAPTER_NUMS]=
{
    {"ui-btnyellowegg.png","ui-btnyelloweggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(0, 0, 0)},
    {"ui-btnbuleegg.png","ui-btnbuleeggSelected.png","ui-btnbuleegg.png","ui-btnpurpleeggSelected.png",Color3B(232, 31, 20)},
    {"ui-btngreenegg.png","ui-btngreeneggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(14, 78, 130)},
    {"ui-btnpurpleegg.png","ui-btnpurpleeggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(65, 23, 139)},
    {"ui-btnbuleegg.png","ui-btnbuleeggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(198, 12, 43)},
    {"ui-btngreenegg.png","ui-btngreeneggSelected.png","ui-btnbuleegg.png","ui-btnbuleeggSelected.png",Color3B(53, 99, 178)}
};

Scene* LevelLayer::scene()
{
	auto sc = Scene::create();
	auto layer = LevelLayer::create();
	sc->addChild(layer);
	return sc;
}

bool LevelLayer::init()
{
	BasicLayer::init();
	initData();
	initView();
	return true;
}


void LevelLayer::initData()
{
    curPageIndex = GameData::getInstance()->getCurPageIndex();
    curPageMaxLevel = GameData::getInstance()->getMaxLevel() - (curPageIndex -1) * EVERY_LEVEL_NUMES;
    curPlayLevel = curPageMaxLevel;
    percentPos = Vec2(0, 100);
}

void LevelLayer::initView()
{
    Node* root = CSLoader::createNode("LevelLayer.csb");
    this->addChild(root);
    
    auto bg = (Sprite*)seekNodeByName(root, "levelBg");
    bgSize = bg->getContentSize();
    
    scView = (ScrollView*)seekNodeByName(root, "scrollView");
    auto innerSize = scView->getInnerContainer()->getContentSize();
    auto scViewSize = scView->getContentSize();
    scViewPos = Vec2(innerSize.width - scViewSize.width,scViewSize.height-innerSize.height);
    auto bFN = btnFramNames[curPageIndex-1];
    for(int i=1;i<=EVERY_LEVEL_NUMES+1;i++)
    {
        const char* btName = String::createWithFormat("button_%02d",i)->getCString();
        auto node = (Button*)seekNodeByName(root, btName);
        auto pos =  convertToNodeSpace(node->getPosition());
        node->addTouchEventListener(CC_CALLBACK_2(LevelLayer::btn_callBack, this));
    
        if(i != EVERY_LEVEL_NUMES+1)
        {
            if(i <= curPageMaxLevel)
            {
                string norFile = bFN.norFile;
                string selFile = bFN.selFile;
                if(i==curPageMaxLevel)
                {
                    norFile = bFN.maxNorFile;
                    norFile = bFN.maxSelFile;
                }
                auto disNorFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(norFile);
                auto disSelFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(selFile);
                //cocosstudi让btn设置为false 再再程序中设置为true才可以替换frame
                node->setScale9Enabled(true);
                node->_buttonNormalRenderer->setSpriteFrame(disNorFrame);
                node->_buttonClickedRenderer->setSpriteFrame(disSelFrame);
            }
            else
            {
                node->setTouchEnabled(false);
                node->_titleRenderer->setVisible(false);
            }
        }
        else
        {
            node = (Button*)seekNodeByName(root, "nextLevel");
            pos =  convertToWorldSpace(node->getPosition());
        }
        
        
        //CCLOG("i = [posX:%f posy:%f tag:%d]",pos.x,pos.y,node->getTag());
        levPoints.push_back(pos);
    }
    
    for(int i=1;i<=EVERY_LEVEL_NUMES;i++)
    {
        const char* dotName = String::createWithFormat("dot%d",i)->getCString();
        auto dot = (Node*)seekNodeByName(root, dotName);
        dot->setColor(bFN.feetColor);
    }
    
    
    hero = (Sprite*)seekNodeByName(root, "hero");
    float hIndex = curPageMaxLevel-1;
    auto hPos = levPoints[hIndex];
    hPos = hPos + Vec2(0, HERO_JUMP_HEIGHT);
    hero->setPosition(hPos);
    
    auto hMove = EaseIn::create(MoveBy::create(0.5f, Vec2(0,30)),0.8f);
    auto hReAct = RepeatForever::create(Sequence::create(hMove,hMove->reverse(), NULL));
    hero->runAction(hReAct);
    
    //移动到目标位置
    //toTargetLevel(curPlayLevel,curPageMaxLevel);
    //scView->scrollToBottomLeft(2.5, true);
    
    auto aimPerVec2 = percentPos - getToPerVec2(levPoints[hIndex]);
    scView->scrollToPercentBothDirection(aimPerVec2,0.5f, true);
    
    
    auto btnNode = (Node*)seekNodeByName(root, "buttonNode");
    curLevSp = Sprite::createWithSpriteFrameName("ui-hore.png");
    curLevSp->setPosition(levPoints[hIndex]);
    curLevSp->setScale(3.0f);
    float time1 = 0.8f;
    auto fadeIn = FadeIn::create(time1);
    auto scale1 = ScaleTo::create(time1, 3.5f);
    auto fadeOut = FadeOut::create(time1*0.5f);
    auto scale2 = ScaleTo::create(time1*0.5f, 2.8f);
    auto spawn1 = Spawn::create(EaseIn::create(fadeIn,0.5),scale1, NULL);
    auto spawn2 = Spawn::create(EaseOut::create(fadeOut,0.5f),scale2, NULL);
    auto reAction = RepeatForever::create(Sequence::create(spawn1,spawn2, NULL));
    curLevSp->runAction(reAction);
    btnNode->addChild(curLevSp);
}

void LevelLayer::btn_callBack(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Audio::getInstance()->playEffect(sound_click);
        CCLOG("btn_callBack");
        Button* btn = (Button*)pSender;
        int level = btn->getTag();
//        toTargetLevel(curPlayLevel,level);
//        curPlayLevel = level;
//        return;
        int trueLevel = (curPageIndex-1) * EVERY_LEVEL_NUMES + level;
        CCLOG("trueLevel:%d",trueLevel);
        GameData::getInstance()->setLevel(trueLevel);
        
        auto dialog =  AimDialog::create();
        addChild(dialog);
    }
}

void LevelLayer::toTargetLevel(int curPlayLevel , int tarLevel,bool isResetHeroPos)
{
    CCLOG("toTargetLevel:%d",tarLevel);
    
//    //到下一章节
//    if(tarLevel % (EVERY_LEVEL_NUMES+1) == 0)
//    {
//        CCLOG("nextChaptor");
//        int cpIndex = GameData::getInstance()->getCurPageIndex();
//        GameData::getInstance()->setCurPageIndex(cpIndex+1);
//        auto sc = LevelLayer::scene();
//        Director::getInstance()->replaceScene(TransitionMoveInR::create(2.5f, sc));
//        return;
//    }
    
    int curPosIndex = curPlayLevel-1 - (curPageIndex-1) * EVERY_LEVEL_NUMES;
    int aimPosIndex = tarLevel-1-(curPageIndex-1) * EVERY_LEVEL_NUMES;
    auto curPos = levPoints[curPosIndex];
    auto aimPos = levPoints[aimPosIndex];
    float dis = curPos.distance(aimPos);
    CCLOG("dis:%f",dis);
    
    if(isResetHeroPos) hero->setPosition(curPos + Vec2(0, HERO_JUMP_HEIGHT));
    if(dis < 10) dis = 10;
    float moveTime = dis * 0.01f;
    auto moveBy = MoveBy::create(moveTime, aimPos-curPos);
    auto callBack = CallFuncN::create(CC_CALLBACK_1(LevelLayer::move_callBack,this));
    auto seq = Sequence::create(moveBy,DelayTime::create(0.2f),callBack, NULL);
    hero->runAction(seq);
    Vec2 perPos = getToPerVec2(aimPos);
    Vec2 curPrePos = percentPos - perPos;
    curPos = scView->getInnerContainer()->getPosition();
    scView->scrollToPercentBothDirection(curPrePos, moveTime, true);

}

void LevelLayer::move_callBack(cocos2d::Ref *pSender)
{
    CCLOG("move_callBack");
    Audio::getInstance()->playEffect(sound_newLevel);
    int index = curPageMaxLevel-1;
    auto pos = levPoints[index];
    if(index < EVERY_LEVEL_NUMES)
    {
        curLevSp->setPosition(pos);
        auto dialog =  AimDialog::create();
        addChild(dialog);
    }
    else
    {
        //下一个章节
        int cpIndex = GameData::getInstance()->getCurPageIndex();
        GameData::getInstance()->setCurPageIndex(cpIndex+1);
//        auto curSc = Director::getInstance()->getRunningScene();
//        auto moveTo = MoveTo::create(2.5f, Vec2(-winWidth,0));
//        curSc->runAction(moveTo);
        auto nextSc = LevelLayer::scene();
        Director::getInstance()->replaceScene(TransitionMoveLR::create(2.5f, nextSc));
    }
}

Vec2 LevelLayer::getToPerVec2(Vec2 tarPos)
{
    int x = MAX(tarPos.x, winWidth / 2);
    int y = MAX(tarPos.y, winHeight/ 2);
    x = MIN(x, bgSize.width - winWidth/2);
    y = MIN(y, bgSize.height - winHeight/2);
    auto goodPoint = Vec2(x, y);
    auto centerOfScreen = winCenter;
    centerOfScreen.subtract(goodPoint);
    float perX = centerOfScreen.x/scViewPos.x*100;
    float perY = centerOfScreen.y/scViewPos.y*100;
    return Vec2(perX , perY);
}