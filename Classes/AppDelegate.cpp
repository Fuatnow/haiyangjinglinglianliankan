#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "LoadingLayer.h"
#include "TestLayer.h"
#include "cocos2d.h"
#include "GameData.h"
USING_NS_CC;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) 
	{
		glview = GLViewImpl::create("My Game");
		glview->setFrameSize(1136, 640);
		director->setOpenGLView(glview);
    }

    Size referenceSize = Size(1136,640);
    Size screenSize = glview->getFrameSize();
    float scale_x = screenSize.width/referenceSize.width;
    float scale_y = screenSize.height/referenceSize.height;
    float desWidth = referenceSize.width;
    float desHeight = screenSize.height/scale_x;
    if(scale_x > scale_y)
    {
        desWidth = screenSize.width/scale_y;
        desHeight = referenceSize.height;
    }
    
    glview->setDesignResolutionSize(desWidth,desHeight,ResolutionPolicy::SHOW_ALL);
    
	std::vector<std::string> paths;
	paths.push_back("image");
    paths.push_back("image/bg");
    paths.push_back("image/item");
	paths.push_back("map");
	paths.push_back("res");
	paths.push_back("particles");
    paths.push_back("audio");
    paths.push_back("level");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
    

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = LoadingLayer::scene();
//    auto scene = TestLayer::createScene();

    // run
    director->runWithScene(scene);

   // getAPPChangel();
    initSDK();
    
    return true;
}

void AppDelegate::initSDK()
{
    //友盟统计
    umeng::MobClickCpp::setLogEnabled(true);
    
    string appChannel = GameData::getInstance()->getAppChanel();
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(UMENG_APPID, appChannel.c_str());
    
    
    //shareSDK 分享
    C2DXShareSDK::open(CCString::create("752080303e12"), false);
    
    //短信
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //QQ空间
    CCDictionary *qzConfigDict = CCDictionary::create();
    qzConfigDict -> setObject(CCString::create("100371282"), "app_id");
    qzConfigDict -> setObject(CCString::create("aed9b0303e3ed1e27bae87c33761161d"), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //微信
    CCDictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create("wx4868b35061f87885"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    CCDictionary *qqConfigDict = CCDictionary::create();
    qqConfigDict -> setObject(CCString::create("100371282"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
   
}
//
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    umeng::MobClickCpp::applicationDidEnterBackground();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    umeng::MobClickCpp::applicationWillEnterForeground();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
