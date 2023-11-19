/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _NODE_LISTENER_H_
#define _NODE_LISTENER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport NodeListener
    {
    public:
        NodeListener();
        virtual ~NodeListener();

    public:
        virtual void NodeUpdated(const Node* pNode) { }
        virtual void NodeDestroyed(const Node* pNode) { }
        virtual void NodeAttached(const Node* pNode) { }
        virtual void NodeDetached(const Node* pNode) { }
    };

}; //LostPeter

#endif