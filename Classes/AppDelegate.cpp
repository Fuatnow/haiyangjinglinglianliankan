#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "HomeLayer.h"
#include "TestLayer.h"
#include "cocos2d.h"
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

bool AppDelegate::applicationDidFinishLaunching() {
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
	paths.push_back("particle");
    paths.push_back("audio");
    paths.push_back("level");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
    

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = HomeLayer::scene();
//    auto scene = TestLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
