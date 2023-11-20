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

#include "../include/VulkanBufferUniform.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanBufferUniform::VulkanBufferUniform(const String& strName, VulkanDevice* pDevice)
        : Base(strName)
        , m_pDevice(pDevice)
        , m_nBufferSize(0)
        , m_nBufferCount(1)
    {
        
    }

    VulkanBufferUniform::~VulkanBufferUniform()
    {
        Destroy();
    }

    void VulkanBufferUniform::Destroy()
    {
        m_pDevice->DestroyVkBuffers(m_aVkUniformBuffer, m_aVkUniformBuffersMemory);
    }

    bool VulkanBufferUniform::Init(uint32_t bufSize, int bufCount)
    {
        m_nBufferSize = bufSize;
        m_nBufferCount = bufCount;

        if (!m_pDevice->CreateVkUniformBuffers(m_nBufferSize, 
                                               m_nBufferCount, 
                                               m_aVkUniformBuffer, 
                                               m_aVkUniformBuffersMemory))
        {
            F_LogError("*********************** VulkanBufferUniform::Init: CreateVkUniformBuffers failed !");
            return false;
        }

        return true;
    }

    bool VulkanBufferUniform::MapMemory(int index, uint32_t size, uint32_t offset, void** ppData)
    {
        if (!m_pDevice->MapVkDeviceMemory(m_aVkUniformBuffersMemory[index],
                                          size,
                                          offset))
        {
            F_LogError("*********************** VulkanBufferUniform::MapMemory: MapVkDeviceMemory failed !");
            return false;
        }

        return true;
    }

    bool VulkanBufferUniform::UnmapMemory(int index)
    {   
        m_pDevice->UnmapVkDeviceMemory(m_aVkUniformBuffersMemory[index]);
        return true;
    }   

}; //LostPeter