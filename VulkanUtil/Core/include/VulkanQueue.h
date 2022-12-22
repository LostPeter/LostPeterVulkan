// author: LostPeter
// time:   2022-12-03

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