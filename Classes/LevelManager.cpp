#include "LevelManager.h"
#include "GameLayer.h"
#include "tinyXML/tinyxml.h"
#include "Tools.h"
bool LevelManager::init()
{
    BasicLayer::init();
    initData();
    initView();
    return true;
}

void LevelManager::initData()
{
    for (int i = 0; i < ROW; i++) mapVec[i].clear();
}

void LevelManager::initView()
{

}

void LevelManager::loadLevel(int level)
{
//    const char* file = String::createWithFormat("%d.tmx",level-1)->getCString();
//    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file).c_str());
//    auto tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
//    auto tileLayer = tileMap->getLayer("map01");
//    auto row = tileMap->getMapSize().width;
//    auto col = tileMap->getMapSize().height;
//    CCLOG("%f %f", row, col);
//    
//    for (int i = 0; i < row + 2; i++)
//    {
//        mapVec[i].resize(col + 2);
//    }
//    
//    for (int i = 1; i <= row; i++)
//    {
//        for (int j = 1; j <= col; j++)
//        {
//            auto tile = tileLayer->getTileAt(Vec2(j-1, i-1));
//            int hasCell = 0;
//            if (tile) hasCell = 1;
//            mapVec[i][j] = hasCell;
//        }
//    }
//    debugMap(row, col);
//    
//    
//    levelInfo.bestScore = level;
//    levelInfo.lenTime = 60;
//    levelInfo.shortTime = 5;
//    levelInfo.kinds = 3;
//    
//    GameLayer* game = (GameLayer*)getParent();
//    Board* board = game->board;
//    board->loadCell(mapVec, levelInfo.kinds);
    
    
//    parseLevel(level);
}


/*!
 *  \brief 通过根节点和节点名获取节点指针。
 *
 *  \param pRootEle   xml文件的根节点。
 *  \param strNodeName  要查询的节点名
 *  \param Node      需要查询的节点指针
 *  \return 是否找到。true为找到相应节点指针，false表示没有找到相应节点指针。
 */
bool GetNodePointerByName(TiXmlElement* pRootEle,std::string &strNodeName,TiXmlElement* &Node)
{
    // 假如等于根节点名，就退出
    if (strNodeName==pRootEle->Value())
    {
        Node = pRootEle;
        return true;
    }
    
    TiXmlElement* pEle = pRootEle;
    for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
    {
        //递归处理子节点，获取节点指针
        if(GetNodePointerByName(pEle,strNodeName,Node))
            return true;
    }
    
    return false;
}

void LevelManager::gainMapInfo(MapInfo &mapInfo , int level)
{
    const char* levelPath1 = String::createWithFormat("CloseNormal.png")->getCString();
//    MessageBox(levelPath1, "levelPath1");
    const char* levelPath = String::createWithFormat("map_%03d.xml",level)->getCString();
//    MessageBox(levelPath, "levelPath");
    string xmlFilePath = FileUtils::getInstance()->fullPathForFilename(levelPath);
//    MessageBox(xmlFilePath.c_str(), "xmlFilePath");
//    //读取的字节数，读取失败则为0
//    ssize_t bffSize;
//    //获取指定地址的内容
//    unsigned char *bff  = FileUtils::getInstance()->getFileData(xmlFilePath,"r", &bffSize);
    
    string str = FileUtils::getInstance()->getStringFromFile(xmlFilePath);
    MessageBox(str.c_str(), "bff");
   
    TiXmlDocument* document = new TiXmlDocument();
    document->Parse(str.c_str(), 0 , TIXML_ENCODING_UTF8);
    
    TiXmlElement* rootElement = document->RootElement();
    
    TiXmlElement* element = NULL;
    
    string elementStr = "Bg";
    bool parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.bg = atoi(element->GetText());
    
    
    elementStr = "ThemeType";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.themeType = atoi(element->GetText());
    
    
    elementStr = "Music";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.music = atoi(element->GetText());
    
    elementStr = "Kinds";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.kinds = atoi(element->GetText());
    
    elementStr = "Map_ROW";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.mapRow = atoi(element->GetText());
    
    elementStr = "Map_COL";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.mapCol = atoi(element->GetText());
    
    
    elementStr = "Cell_Width";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.cellWidth = atoi(element->GetText());
    
    
    elementStr = "Cell_Height";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.cellHeight = atoi(element->GetText());
    
    elementStr = "ISIncreace";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.isIncrease = atoi(element->GetText());
    
    
    elementStr = "UnLockProp";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.unLockType = atoi(element->GetText());
    
    
    elementStr = "Gravity";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    TiXmlElement* childEle = element->FirstChildElement();
    while (childEle)
    {
        TiXmlElement* eleTemp = NULL;
        elementStr = "X";
        parseFlag = GetNodePointerByName(childEle,elementStr,eleTemp);
        CCASSERT(parseFlag, "parseErr");
        int x = atoi(eleTemp->GetText());
        
        elementStr = "Y";
        parseFlag = GetNodePointerByName(childEle,elementStr,eleTemp);
        CCASSERT(parseFlag, "parseErr");
        int y = atoi(eleTemp->GetText());

        Pos pos = {x,y};
        
        mapInfo.gravityPosVec.push_back(pos);
        
        childEle = childEle->NextSiblingElement();
    }
    
    elementStr = "Map_Array";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.mapArray.resize(mapInfo.mapRow+2);
    for (int i = 0; i < mapInfo.mapRow + 2; i++)
    {
        mapInfo.mapArray[i].resize(mapInfo.mapCol + 2);
    }
    childEle = element->FirstChildElement();
    int row = 1;
    while (childEle)
    {
        TiXmlElement* eleTemp = NULL;
        elementStr = "Array";
        parseFlag = GetNodePointerByName(childEle,elementStr,eleTemp);
        CCASSERT(parseFlag, "parseErr");
        string str = eleTemp->GetText();
        vector<string> arrVec;
        split(arrVec,str,",");
        
        int col=1;
        for(vector<string>::iterator iter = arrVec.begin() ; iter!= arrVec.end();iter++)
        {
            mapInfo.mapArray[row][col] = atoi((*iter).c_str());
            col++;
        }
        childEle = childEle->NextSiblingElement();
        row++;
    }
    
    elementStr = "Aim_Pass";
    parseFlag = GetNodePointerByName(rootElement,elementStr,element);
    CCASSERT(parseFlag, "parseErr");
    
    elementStr = "DiscriPtion";
    parseFlag = GetNodePointerByName(element,elementStr,childEle);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.aimpassInfo.discription = childEle->GetText();
    
    elementStr = "PassType";
    parseFlag = GetNodePointerByName(element,elementStr,childEle);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.aimpassInfo.passType = atoi(childEle->GetText());
    
    elementStr = "LenTime";
    parseFlag = GetNodePointerByName(element,elementStr,childEle);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.aimpassInfo.lenTime = atof(childEle->GetText());
    
    elementStr = "ShorTime";
    parseFlag = GetNodePointerByName(element,elementStr,childEle);
    CCASSERT(parseFlag, "parseErr");
    mapInfo.aimpassInfo.shortTime = atof(childEle->GetText());
    
    delete document;
}

void LevelManager::debugMap(int row,int col)
{
    for (int i = 0; i < row +2; i++)
    {
        string str = "";
        for (int j = 0; j < col+2; j++)
        {
            if (mapVec[j][i] == 1) str += " X ";
            else if (mapVec[j][i] == 2) str += " | ";
            else str += " | ";
        }
        CCLOG("%s",str.c_str());
    }
    /******************************************
     输出类似这样
     |  |  |  |  |  |  |  |  |  |
     |  X  X  X  X  X  X  X  X  |
     |  X  X  X  X  X  X  X  X  |
     |  X  X  X  X  X  X  X  X  |
     |  |  |  |  |  |  |  |  |  |
     |  |  |  |  |  |  |  |  |  |
     |  X  X  X  X  X  X  X  X  |
     |  X  X  X  X  X  X  X  X  |
     |  X  X  X  X  X  X  X  X  |
     |  |  |  |  |  |  |  |  |  |
	***/
}

void LevelManager::split(std::vector<std::string> &result,std::string str,std::string pattern)
{
    std::string::size_type pos;
    str+=pattern;//扩展字符串以方便操作
    unsigned int size=str.size();
    for(unsigned int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return;
}


//
//
///*!
//*  \brief 通过节点查询
//*
//*  \param XmlFile   xml文件全路径。
//*  \param strNodeName  要查询的节点名
//*  \param strText      要查询的节点文本
//*  \return 是否成功。true为成功，false表示失败。
//*/
//
//
//bool QueryNode_Text(const char* XmlFile,std::string strNodeName,std::string &strText)
//{
//    // 定义一个TiXmlDocument类指针
//    TiXmlDocument *pDoc = new TiXmlDocument();
//    if (NULL==pDoc)
//    {
//        return false;
//    }
//    pDoc->LoadFile(XmlFile);
//    TiXmlElement *pRootEle = pDoc->RootElement();
//    if (NULL==pRootEle)
//    {
//        return false;
//    }
//    
//    TiXmlElement *pNode = NULL;
//    GetNodePointerByName(pRootEle,strNodeName,pNode);
//    if (NULL!=pNode)
//    {
//        const char* psz = pNode->GetText();
//        if (NULL==psz)
//        {
//            strText = "";
//        }
//        else
//        {
//            strText = psz;
//        }
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//    
//}
//
//
///*!
// *  \brief 通过节点查询。
// *
// *  \param XmlFile   xml文件全路径。
// *  \param strNodeName  要查询的节点名
// *  \param AttMap      要查询的属性值，这是一个map，前一个为属性名，后一个为属性值
// *  \return 是否成功。true为成功，false表示失败。
// */
//bool QueryNode_Attribute(const char* XmlFile,std::string strNodeName,std::map<std::string,std::string> &AttMap)
//{
//    // 定义一个TiXmlDocument类指针
//    typedef std::pair <std::string,std::string> String_Pair;
//    
//    TiXmlDocument *pDoc = new TiXmlDocument();
//    if (NULL==pDoc)
//    {
//        return false;
//    }
//    
//    pDoc->LoadFile(XmlFile);
//    TiXmlElement *pRootEle = pDoc->RootElement();
//    if (NULL==pRootEle)
//    {
//        return false;
//    }
//    
//    TiXmlElement *pNode = NULL;
//    
//    GetNodePointerByName(pRootEle,strNodeName,pNode);
//    
//    if (NULL!=pNode)
//    {
//        TiXmlAttribute* pAttr = NULL;
//        for (pAttr = pNode->FirstAttribute(); pAttr; pAttr = pAttr->Next())
//        {
//            std::string strAttName = pAttr->Name();
//            std::string strAttValue = pAttr->Value();
//            AttMap.insert(String_Pair(strAttName,strAttValue));
//        }
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//    return true;
//}
