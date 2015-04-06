#ifndef BasicLayer_H_
#define BasicLayer_H_
#include "cocos2d.h"
USING_NS_CC;
class BasicLayer : public Layer
{
public:
	BasicLayer(){}
	virtual ~BasicLayer(){}
	virtual bool init();
	CREATE_FUNC(BasicLayer);
public:
	Size winSize;
	Point winCenter;
	float winWidth;
	float winHeight;
	float ppuScale;
};
#endif