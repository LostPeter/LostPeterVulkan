/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Viewport.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    Viewport::Viewport(const String& nameViewport)
        : Base(nameViewport)
    {

    }
    Viewport::~Viewport()
    {
        Destroy();
    }

    void Viewport::Destroy()
    {

    }

    bool Viewport::Init(float vpStartX, 
                        float vpStartY, 
                        float vpWidth, 
                        float vpHeight,
                        float vpMinDepth,
                        float vpMaxDepth,
                        VkOffset2D scOffset,
                        VkExtent2D scExtent)
    {
        AddViewport(vpStartX,
                    vpStartY,
                    vpWidth,
                    vpHeight,
                    vpMinDepth,
                    vpMaxDepth,
                    scOffset,
                    scExtent);

        return true;
    }

    void Viewport::AddViewport(float vpStartX, 
                               float vpStartY, 
                               float vpWidth, 
                               float vpHeight,
                               float vpMinDepth,
                               float vpMaxDepth,
                               VkOffset2D scOffset,
                               VkExtent2D scExtent)
    {
        VkViewport vkViewport = {};
        vkViewport.x = vpStartX;
        vkViewport.y = vpStartY;
        vkViewport.width = vpWidth;
        vkViewport.height = vpHeight;
        vkViewport.minDepth = vpMinDepth;
        vkViewport.maxDepth = vpMaxDepth;

        VkRect2D vkScissor = {};
        vkScissor.offset = scOffset;
        vkScissor.extent = scExtent;

        m_aViewports.push_back(vkViewport);
        m_aScissors.push_back(vkScissor);
    }

    void Viewport::RefreshViewport(int index, const VkExtent2D& vkExtent)
    {
        if (index < 0 || index >= (int)m_aViewports.size())
            return;

        m_aViewports[index].width = (float)vkExtent.width;
        m_aViewports[index].height = (float)vkExtent.height;
        m_aScissors[index].extent = vkExtent;
    }

}; //LostPeter