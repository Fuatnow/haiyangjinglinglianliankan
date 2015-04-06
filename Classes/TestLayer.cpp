//
//  TestLayer.cpp
//  Link
//
//  Created by Fuatnow on 15-4-4.
//
//

#include "TestLayer.h"
#include "tinyXML/tinyxml.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


Scene* TestLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TestLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool TestLayer::init()
{
    Layer::init();
    initView();
    return true;
}

void TestLayer::initView()
{
    CCLOG("initView");
    
    for(int level = 0 ; level < 150 ; level++)
    {
        
        string resPath = "/Users/fuatnow/cocos2d-x-3.3/projects/Link/Resources/level/";
        const char* flieName = String::createWithFormat("map_%03d.xml",level+1)->getCString();
        string flieNamePath = resPath + string(flieName);
        
        const char* file = String::createWithFormat("%d.tmx",level+100)->getCString();
        auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file).c_str());
        auto tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
        auto tileLayer = tileMap->getLayer("map01");
        auto row = tileMap->getMapSize().width;
        auto col = tileMap->getMapSize().height;
        CCLOG("%f %f", row, col);
        
        TiXmlDocument* document = new TiXmlDocument();
        document->LoadFile();
        
        
        TiXmlComment *pComment=new TiXmlComment("copyright by fuatnow.");
        document->LinkEndChild(pComment);
        
        
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
        document->LinkEndChild(decl);
        
        
        TiXmlElement* rootElement = new TiXmlElement("Map");
        document->LinkEndChild(rootElement);
        
        //0. theme style
        TiXmlElement* themeTypeElement = new TiXmlElement("ThemeType");
        TiXmlText* themeTypeContent = new TiXmlText("1");
        themeTypeElement->LinkEndChild(themeTypeContent);
        rootElement->LinkEndChild(themeTypeElement);
        
        
        //1. bg type
        TiXmlElement* bgTypeElement = new TiXmlElement("Bg");
        TiXmlText* bgContent = new TiXmlText("1");
        bgTypeElement->LinkEndChild(bgContent);
        rootElement->LinkEndChild(bgTypeElement);
        
        //2. muisc type
        TiXmlElement* musicTypeElement = new TiXmlElement("Music");
        TiXmlText* musicContent = new TiXmlText("1");
        musicTypeElement->LinkEndChild(musicContent);
        rootElement->LinkEndChild(musicTypeElement);
        
        
        //3. kinds
        int kinds = level % 13 + 3;
        TiXmlElement* cellKindsElement = new TiXmlElement("Kinds");
        std::stringstream ss;
        ss << kinds ;
        TiXmlText* kindsContent = new TiXmlText(ss.str().c_str());
        cellKindsElement->LinkEndChild(kindsContent);
        rootElement->LinkEndChild(cellKindsElement);
        
        //4. map_width
        TiXmlElement* mapWidthElement = new TiXmlElement("Map_ROW");
        ss.clear();
        ss.str("");
        ss << row;
        TiXmlText* mapWidthContent = new TiXmlText(ss.str().c_str());
        mapWidthElement->LinkEndChild(mapWidthContent);
        rootElement->LinkEndChild(mapWidthElement);
        
        //5. map_height
        ss.clear();
        ss.str("");
        ss << col;
        TiXmlElement* mapHeightElement = new TiXmlElement("Map_COL");
        TiXmlText* mapHeightContent = new TiXmlText(ss.str().c_str());
        mapHeightElement->LinkEndChild(mapHeightContent);
        rootElement->LinkEndChild(mapHeightElement);
        
        //6. cell_width
        TiXmlElement* cellWidthElement = new TiXmlElement("Cell_Width");
        TiXmlText* cellWidthContent = new TiXmlText("72");
        cellWidthElement->LinkEndChild(cellWidthContent);
        rootElement->LinkEndChild(cellWidthElement);
        
        //7. cell_height
        TiXmlElement* cellHeightElement = new TiXmlElement("Cell_Height");
        TiXmlText* cellHeightContent = new TiXmlText("72");
        cellHeightElement->LinkEndChild(cellHeightContent);
        rootElement->LinkEndChild(cellHeightElement);
        
        
        //8
        TiXmlElement* isIncreaceElement = new TiXmlElement("ISIncreace");
        TiXmlText* isIncreaceContent = new TiXmlText("1");
        isIncreaceElement->LinkEndChild(isIncreaceContent);
        rootElement->LinkEndChild(isIncreaceElement);
        
        
        //9
        TiXmlElement* unLockPropElement = new TiXmlElement("UnLockProp");
//        unLockPropElement->SetAttribute("isUnLock", "0");
//        
//        TiXmlElement* unLockTypeElement = new TiXmlElement("unLockType");
        TiXmlText* unLockTypeContent = new TiXmlText("0");
        unLockPropElement->LinkEndChild(unLockTypeContent);
//
//        unLockPropElement->LinkEndChild(unLockTypeElement);
        rootElement->LinkEndChild(unLockPropElement);
        
        
        //10
        TiXmlElement* gravityElement = new TiXmlElement("Gravity");
        gravityElement->SetAttribute("num", "0");
        
        for(int i=0 ; i<2 ;i++)
        {
            TiXmlElement* gravityPosElement = new TiXmlElement("Pos");
            
            TiXmlElement* gravityPosXElement = new TiXmlElement("X");
            TiXmlText* gravityPosXContent = new TiXmlText("0");
            gravityPosXElement->LinkEndChild(gravityPosXContent);
            TiXmlElement* gravityPosYElement = new TiXmlElement("Y");
            TiXmlText* gravityPosYContent = new TiXmlText("0");
            gravityPosYElement->LinkEndChild(gravityPosYContent);
            
            gravityPosElement->LinkEndChild(gravityPosXElement);
            gravityPosElement->LinkEndChild(gravityPosYElement);
            
            gravityElement->LinkEndChild(gravityPosElement);
            rootElement->LinkEndChild(gravityElement);
        }
        
        //11
        TiXmlElement* mapArrayElement = new TiXmlElement("Map_Array");
        for (int i = 1; i <= row; i++)
        {
            string str = "";
            for (int j = 1; j <= col; j++)
            {
                auto tile = tileLayer->getTileAt(Vec2(j-1, i-1));
                int hasCell = 0;
                if (tile) hasCell = 1;
                //            mapVec[i][j] = hasCell;
                std::stringstream ss;
                ss << hasCell ;
                str += ss.str();
                if(j != col) str += " , ";
            }
            TiXmlElement* arrayElement = new TiXmlElement("Array");
            TiXmlText* arrayContent = new TiXmlText(str.c_str());
            arrayElement->LinkEndChild(arrayContent);
            mapArrayElement->LinkEndChild(arrayElement);
        }
        rootElement->LinkEndChild(mapArrayElement);
        
        
        //12 Aim_Pass
        TiXmlElement* aimPassElement = new TiXmlElement("Aim_Pass");
        
        TiXmlElement* discrableElement = new TiXmlElement("DiscriPtion");
        TiXmlText* discrableContent = new TiXmlText("规定的时间内消除所有方格");
        discrableElement->LinkEndChild(discrableContent);
        
        TiXmlElement* passTypeElement = new TiXmlElement("PassType");
        TiXmlText* passTypeContent = new TiXmlText("1");
        passTypeElement->LinkEndChild(passTypeContent);
        
        TiXmlElement* lenTimeElement = new TiXmlElement("LenTime");
        TiXmlText* lenTimeContent = new TiXmlText("60");
        lenTimeElement->LinkEndChild(lenTimeContent);
        
        TiXmlElement* shortTimeElement = new TiXmlElement("ShorTime");
        TiXmlText* shortTimeContent = new TiXmlText("2");
        shortTimeElement->LinkEndChild(shortTimeContent);
        
        aimPassElement->LinkEndChild(discrableElement);
        aimPassElement->LinkEndChild(passTypeElement);
        aimPassElement->LinkEndChild(lenTimeElement);
        aimPassElement->LinkEndChild(shortTimeElement);
        
        rootElement->LinkEndChild(aimPassElement);
        document->SaveFile(flieNamePath.c_str());
        
        delete document;
//        ofstream fout(flieNamePath);
//        fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?> ";
        

        
//        string xmlFilePath = FileUtils::getInstance()->fullPathForFilename("zombies.xml");
//        //读取的字节数，读取失败则为0
//        long bffSize =   0;
//        //获取指定地址的内容
//        unsigned char* bff  =   FileUtils::getInstance()->getFileData(xmlFilePath.c_str(),"r", &bffSize);
//        TiXmlDocument* document = new TiXmlDocument();
//        document->Parse((char*)bff , 0 , TIXML_ENCODING_UTF8);

        

    }
}