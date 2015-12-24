//
//  Tools.h
//  Link
//
//  Created by Fuatnow on 15-3-24.
//
//

#ifndef __Link__Tools__
#define __Link__Tools__
#include "cocos2d.h"
USING_NS_CC;

typedef enum
{
    k_Operate_Pause = 0,
    k_Operate_Resume
}OperateFlag;

static Node* seekNodeByName(Node* root, const std::string& name)
{
    if (!root)
    {
        return nullptr;
    }
    if (root->getName() == name)
    {
        return root;
    }
    const auto& arrayRootChildren = root->getChildren();
    for (auto& subWidget : arrayRootChildren)
    {
        Node* child = dynamic_cast<Node*>(subWidget);
        if (child)
        {
            Node* res = seekNodeByName(child,name);
            if (res != nullptr)
            {
                return res;
            }
        }
    }
    return nullptr;
}


static void operateAllSchedulerAndActions(cocos2d::Node* node, OperateFlag flag)
{
    if(node->isRunning())
    {
        switch (flag)
        {
            case k_Operate_Pause:
                node->pause();
                break;
            case k_Operate_Resume:
                node->resume();
                break;
            default:
                break;
        }
        auto array = node->getChildren();
        ssize_t count = array.size();
        for(ssize_t i=0;i<count;i++)
        {
            auto child = array.at(i);
            operateAllSchedulerAndActions(child, flag);
        }
    }
}

#endif /* defined(__Link__Tools__) */
