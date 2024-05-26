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

#include "../include/RHIVulkanBindGroup.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanBindGroupPool.h"
#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanBuffer.h"
#include "../include/RHIVulkanBufferView.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanTextureView.h"
#include "../include/RHIVulkanSampler.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroup::RHIVulkanBindGroup(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupCreateInfo& createInfo)
        : RHIBindGroup(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanBindGroupPool((RHIVulkanBindGroupPool*)createInfo.pBindGroupPool)
        , m_pVulkanBindGroupLayout((RHIVulkanBindGroupLayout*)createInfo.pBindGroupLayout)
        , m_vkDescriptorSet(VK_NULL_HANDLE)
        , m_aBindGroupEntries(createInfo.aEntries)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanBindGroupPool && m_pVulkanBindGroupLayout && "RHIVulkanBindGroup::RHIVulkanBindGroup")

        createVkDescriptorSet();
    }

    RHIVulkanBindGroup::~RHIVulkanBindGroup()
    {
        Destroy();
    }

    void RHIVulkanBindGroup::Destroy()
    {
        if (m_vkDescriptorSet != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->FreeVkDescriptorSets(m_pVulkanBindGroupPool->GetVkDescriptorPool(), m_vkDescriptorSet);
        }
        m_vkDescriptorSet = VK_NULL_HANDLE;
    }

    void RHIVulkanBindGroup::UpdateVkDescriptorSets()
    {
        VkWriteDescriptorSetVector aWriteDescriptorSets;
        uint32 count = (uint32)m_aBindGroupEntries.size();
        for (uint32 i = 0; i < count; i++)
        {
            RHIBindGroupEntry& entry = m_aBindGroupEntries[i];

            VkDescriptorType vkDescriptorType = RHIVulkanConverter::TransformToVkDescriptorType(entry.sBinding.eBinding);
            if (entry.sBinding.eBinding == RHIBindingType::RHI_Binding_UniformBuffer ||
                entry.sBinding.eBinding == RHIBindingType::RHI_Binding_StorageBuffer)
            {
                RHIVulkanBufferView* pVulkanBufferView = (RHIVulkanBufferView*)entry.pBufferView;
                F_Assert(pVulkanBufferView && "RHIVulkanBindGroup::UpdateVkDescriptorSets")

                m_pVulkanDevice->PushVkDescriptorSet_Buffer(aWriteDescriptorSets, 
                                                            m_vkDescriptorSet,
                                                            (uint32_t)entry.sBinding.sBinding.sBindingHLSL.nIndex,
                                                            0,
                                                            1,
                                                            vkDescriptorType,
                                                            pVulkanBufferView->GetVkDescriptorBufferInfo());
            }
            else if (entry.sBinding.eBinding == RHIBindingType::RHI_Binding_Sampler)
            {
                RHIVulkanSampler* pVulkanSampler = (RHIVulkanSampler*)entry.pSampler;
                F_Assert(pVulkanSampler && "RHIVulkanBindGroup::UpdateVkDescriptorSets")

                VkDescriptorImageInfo imageInfo = {};
                imageInfo.sampler = pVulkanSampler->GetVkSampler();
                
                m_pVulkanDevice->PushVkDescriptorSet_Image(aWriteDescriptorSets, 
                                                           m_vkDescriptorSet,
                                                           (uint32_t)entry.sBinding.sBinding.sBindingHLSL.nIndex,
                                                           0,
                                                           1,
                                                           vkDescriptorType,
                                                           imageInfo);
            }
            else if (entry.sBinding.eBinding == RHIBindingType::RHI_Binding_CombinedImageSampler ||
                     entry.sBinding.eBinding == RHIBindingType::RHI_Binding_Texture ||
                     entry.sBinding.eBinding == RHIBindingType::RHI_Binding_StorageTexture)
            {
                RHIVulkanTextureView* pVulkanTextureView = (RHIVulkanTextureView*)entry.pTextureView;
                RHIVulkanSampler* pVulkanSampler = (RHIVulkanSampler*)entry.pSampler;

                VkDescriptorImageInfo imageInfo = {};
                if (pVulkanSampler != nullptr)
                {
                    imageInfo.sampler = pVulkanSampler->GetVkSampler();
                }
                imageInfo.imageView = pVulkanTextureView->GetVkImageView();
                imageInfo.imageLayout = pVulkanTextureView->GetVulkanTexture()->GetVkImageLayout();
                
                m_pVulkanDevice->PushVkDescriptorSet_Image(aWriteDescriptorSets, 
                                                           m_vkDescriptorSet,
                                                           (uint32_t)entry.sBinding.sBinding.sBindingHLSL.nIndex,
                                                           0,
                                                           1,
                                                           vkDescriptorType,
                                                           imageInfo);
            }
            else
            {
                F_LogError("*********************** RHIVulkanBindGroup::UpdateVkDescriptorSets: Wrong RHIBindingType, Name: [%s] !", m_strName.c_str());
                return;
            }
        }
        m_pVulkanDevice->UpdateVkDescriptorSets(aWriteDescriptorSets);
    }
    
    void RHIVulkanBindGroup::createVkDescriptorSet()
    {   
        if (!m_pVulkanDevice->CreateVkDescriptorSet(1,
                                                    m_pVulkanBindGroupLayout->GetVkDescriptorSetLayout(),
                                                    m_pVulkanBindGroupPool->GetVkDescriptorPool(),
                                                    m_vkDescriptorSet))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanBindGroup::createVkDescriptorSet: Wrong CreateVkDescriptorSet failed, Name: [%s] !", m_strName.c_str());
            return;
        }
        UpdateVkDescriptorSets();

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_DESCRIPTOR_SET, reinterpret_cast<uint64_t>(m_vkDescriptorSet), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanBindGroup::createVkDescriptorSet: Create VkDescriptorSet success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan