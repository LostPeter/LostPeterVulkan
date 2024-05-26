/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanSampler.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroupLayout::RHIVulkanBindGroupLayout(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupLayoutCreateInfo& createInfo)
        : RHIBindGroupLayout(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkDescriptorSetLayout(VK_NULL_HANDLE)
        , m_nLayoutIndex(createInfo.nLayoutIndex)
        , m_aBindGroupLayoutEntries(createInfo.aEntries)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroupLayout::RHIVulkanBindGroupLayout")

        createVkDescriptorSetLayout();
    }   

    RHIVulkanBindGroupLayout::~RHIVulkanBindGroupLayout()
    {
        Destroy();
    }

    void RHIVulkanBindGroupLayout::Destroy()
    {
        if (m_vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkDescriptorSetLayout(m_vkDescriptorSetLayout);
        }
        m_vkDescriptorSetLayout = VK_NULL_HANDLE;
        m_aDescriptorSetLayoutBinding.clear();
    }
    
    void RHIVulkanBindGroupLayout::createVkDescriptorSetLayout()
    {
        uint32 count = (uint32)m_aBindGroupLayoutEntries.size();
        for (uint32 i = 0; i < count; i++)
        {
            RHIBindGroupLayoutEntry& entry = m_aBindGroupLayoutEntries[i];

            VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
            VkDescriptorType vkDescriptorType = RHIVulkanConverter::TransformToVkDescriptorType(entry.sBinding.eBinding);
            VkShaderStageFlags vkShaderStageFlags = RHIVulkanConverter::TransformToVkShaderStageFlagsFromShaderStagelags(entry.flagsShaderVisibility);
            if (entry.sBinding.eBinding == RHIBindingType::RHI_Binding_UniformBuffer ||
                entry.sBinding.eBinding == RHIBindingType::RHI_Binding_StorageBuffer)
            {
                m_pVulkanDevice->CreateVkDescriptorSetLayoutBinding_Buffer(entry.sBinding.sBinding.sBindingHLSL.nIndex,
                                                                           vkDescriptorType,
                                                                           1,
                                                                           vkShaderStageFlags,
                                                                           vkDescriptorSetLayoutBinding);
                
            }
            else if (entry.sBinding.eBinding == RHIBindingType::RHI_Binding_Sampler ||
                     entry.sBinding.eBinding == RHIBindingType::RHI_Binding_Texture ||
                     entry.sBinding.eBinding == RHIBindingType::RHI_Binding_StorageTexture)
            {
                VkSampler* pVkSampler = nullptr;
                if (entry.pSampler != nullptr)
                {
                    RHIVulkanSampler* pVulkanSampler = (RHIVulkanSampler*)entry.pSampler;
                    pVkSampler = pVulkanSampler->GetVkSamplerPtr();
                }
                m_pVulkanDevice->CreateVkDescriptorSetLayoutBinding_Image(entry.sBinding.sBinding.sBindingHLSL.nIndex,
                                                                          vkDescriptorType,
                                                                          1,
                                                                          vkShaderStageFlags,
                                                                          pVkSampler,
                                                                          vkDescriptorSetLayoutBinding);
            }   
            else
            {
                setIsError(true);
                F_LogError("*********************** RHIVulkanBindGroupLayout::createVkDescriptorSetLayout: Wrong RHIBindingType, Name: [%s] !", m_strName.c_str());
                return;
            }
            m_aDescriptorSetLayoutBinding.push_back(vkDescriptorSetLayoutBinding);
        }
        
        if (m_aDescriptorSetLayoutBinding.empty() ||
            !m_pVulkanDevice->CreateVkDescriptorSetLayout(m_aDescriptorSetLayoutBinding, m_vkDescriptorSetLayout))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanBindGroupLayout::createVkDescriptorSetLayout: Wrong CreateVkDescriptorSetLayout failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, reinterpret_cast<uint64_t>(m_vkDescriptorSetLayout), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanBindGroupLayout::createVkDescriptorSetLayout: Create VkDescriptorSetLayout success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan