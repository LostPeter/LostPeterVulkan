/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_QUEUE_H_
#define _VULKAN_QUEUE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanQueue
    {
    public:
        VulkanQueue();
        ~VulkanQueue();

    public:
    protected:
        VkQueue m_vkQueue;
        uint32 m_nFamilyIndex;
        VulkanDevice* m_pDevice;

    public:
        const VkQueue& GetVkQueue() const { return m_vkQueue; }
        uint32 GetFamilyIndex() const { return m_nFamilyIndex; }
            
    public:
        void Destroy();
        bool Init(VulkanDevice* pDevice, uint32 nFamilyIndex);
    };

}; //LostPeter

#endif