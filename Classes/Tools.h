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

#endif /* defined(__Link__Tools__) */
