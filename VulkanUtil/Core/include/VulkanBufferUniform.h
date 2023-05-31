/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_BUFFER_UNIFORM_H_
#define _VULKAN_BUFFER_UNIFORM_H_

#include "VulkanObject.h"

namespace LostPeter
{
    class utilExport VulkanBufferUniform : public VulkanObject
    {
    public:
        VulkanBufferUniform(const String& strName);
        virtual ~VulkanBufferUniform();

    public:
    protected:
        uint32_t m_nBufferSize;
        int m_nBufferCount;

        VkBufferVector m_aVkUniformBuffer;
        VkDeviceMemoryVector m_aVkUniformBuffersMemory;
        
    public:
        UTIL_FORCEINLINE const VkBufferVector& GetVkBufferVector() const { return m_aVkUniformBuffer; }
        UTIL_FORCEINLINE VkBufferVector& GetVkBufferVector() { return m_aVkUniformBuffer; }
        UTIL_FORCEINLINE const VkDeviceMemoryVector& GetVkDeviceMemoryVector() const { return m_aVkUniformBuffersMemory; }
        UTIL_FORCEINLINE VkDeviceMemoryVector& GetVkDeviceMemoryVector() { return m_aVkUniformBuffersMemory; }

    public:
        void Destroy();
        bool Init(uint32_t bufSize, int bufCount);

        bool MapMemory(int index, uint32_t size, uint32_t offset, void** ppData);
        bool UnmapMemory(int index);
    };

}; //LostPeter

#endif