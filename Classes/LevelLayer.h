#ifndef LevelLayer_H__
#define LevelLayer_H__
#include "BasicLayer.h"
#include "Define.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Tools.h"
#include "Define.h"
#include "TransitionMoveLR.h"
#include "TransitionMoveRL.h"
#include "PackageLayer.h"
#include "RankLayer.h"
#include "ShopLayer.h"
using namespace ui;
using namespace std;

struct Btn_FrameName
{
    string norFile;
    string selFile;
    string maxNorFile;
    string maxSelFile;
    Color3B feetColor;
};

class LevelLayer : public BasicLayer
{
public:
	LevelLayer(){};
	~LevelLayer(){};
	static Scene* scene();
	virtual bool init();
	void initData();
	void initView();
    void preChap_callBack(Ref *pSender, Widget::TouchEventType type);
    void btn_callBack(Ref *pSender, Widget::TouchEventType type);
    void menuBtn_callBack(Ref *pSender, Widget::TouchEventType type);
    void toTargetLevel(int curPlayLevel , int tarLevel,bool isResetHeroPos=true);
    Vec2 getToPerVec2(Vec2 tarPos);
    void move_callBack(Ref* pSender);
    void btnPop_callBack(Node* pSender);
    void lock_callBack(Node* pSender);
    void popMenuBtn();
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	CREATE_FUNC(LevelLayer);
public:
    Size bgSize;
    bool hasPopBtn;
    bool isBtnMoveing;
    int popBtnNum;
    int curPageIndex;
    int curPageMaxLevel;
    int curPlayLevel;
    ScrollView* scView;
    Sprite* hero;
    vector<Vec2> levPoints;
    vector<Vec2> btnPoints;
    //当前scViewPos  也就是innerlayer多出view部分  宽度为正 高度为负
    Vec2 scViewPos;
    //当前scview占得百分比
    Vec2 percentPos;
    Sprite* curLevSp;
    Button* menuBtn;
    Button* shopBtn;
    Button* packBtn;
    Button* friendBtn;
    Button* discountBtn;
};
#endif