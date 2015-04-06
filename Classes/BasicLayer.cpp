#include "BasicLayer.h"
bool BasicLayer::init()
{
	Layer::init();
	winSize = Director::getInstance()->getWinSize();
	winWidth = winSize.width;
	winHeight = winSize.height;
	winCenter = Point(winWidth / 2, winHeight / 2);
	float ppux = winWidth / 1280;
	float ppuy = winHeight / 720;
	ppuScale = ((ppux > ppuy) ? ppux : ppuy);
	return true;
}