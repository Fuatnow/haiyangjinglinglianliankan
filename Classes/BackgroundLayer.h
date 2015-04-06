#ifndef BackgroudLayer_H_
#define BackgroudLayer_H_
#include "BasicLayer.h"
#include "cocostudio/CocoStudio.h"
class BackgroundLayer : public BasicLayer
{
public:
	BackgroundLayer(){}
	~BackgroundLayer(){}
	virtual bool init();
	void initView();
	CREATE_FUNC(BackgroundLayer);
};
#endif // !BackgroudLayer_H_
