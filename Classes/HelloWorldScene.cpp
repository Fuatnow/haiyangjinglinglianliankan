#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	LayerColor* bgLayer = LayerColor::create(Color4B(255, 255, 255, 255));
	addChild(bgLayer);
	initView();
	return true;
}

void HelloWorld::initView()
{
	/*std::string file = "100.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
	_background = _tileMap->layerNamed("map01");
	auto width = _tileMap->getMapSize().width;
	auto height = _tileMap->getMapSize().height;
	CCLOG("%f %f", width, height);
	auto tile1 = _background->tileAt(Vec2(0, 3));
	tile1->runAction(MoveTo::create(5, Vec2(800, 100)));
	addChild(_tileMap);*/
}