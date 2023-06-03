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

#include "../include/VulkanBufferUniform.h"
#include "../include/VulkanDefine.h"
#include "../include/VulkanManager.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanBufferUniform::VulkanBufferUniform(const String& strName)
        : VulkanObject(strName)
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
        VulkanObject::ms_pVulkanManager->GetDevice()->DestroyVkBuffers(m_aVkUniformBuffer, m_aVkUniformBuffersMemory);
    }

    bool VulkanBufferUniform::Init(uint32_t bufSize, int bufCount)
    {
        m_nBufferSize = bufSize;
        m_nBufferCount = bufCount;

        if (!VulkanObject::ms_pVulkanManager->GetDevice()->CreateVkUniformBuffers(m_nBufferSize, 
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
        if (!VulkanObject::ms_pVulkanManager->GetDevice()->MapVkDeviceMemory(m_aVkUniformBuffersMemory[index],
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
        VulkanObject::ms_pVulkanManager->GetDevice()->UnmapVkDeviceMemory(m_aVkUniformBuffersMemory[index]);
        return true;
    }   

}; //LostPeter