/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport Renderable : public Base
    {
    public:
        Renderable(const String& nameRenderable);
        virtual ~Renderable();

    public:
    protected:
        VulkanRenderQueueType typeRenderQueue;

    public:
        LP_FORCEINLINE VulkanRenderQueueType GetRenderQueueType() const { return this->typeRenderQueue; }

    public:
        
    };

}; //LostPeterEngine

#endif