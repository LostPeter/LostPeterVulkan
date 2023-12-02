/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_QUEUE_H_
#define _VULKAN_QUEUE_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanQueue
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
        E_FORCEINLINE const VkQueue& GetVkQueue() const { return m_vkQueue; }
        E_FORCEINLINE uint32 GetFamilyIndex() const { return m_nFamilyIndex; }
            
    public:
        void Destroy();
        bool Init(VulkanDevice* pDevice, uint32 nFamilyIndex);
    };

}; //LostPeter

#endif