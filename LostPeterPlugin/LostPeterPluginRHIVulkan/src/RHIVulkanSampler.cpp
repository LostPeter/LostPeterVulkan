/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-09
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanSampler.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanSampler::RHIVulkanSampler(RHIVulkanDevice* pVulkanDevice, const RHISamplerCreateInfo& createInfo)
        : RHISampler(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkSampler(VK_NULL_HANDLE)
        , m_eAddressU(createInfo.eAddressU)
        , m_eAddressV(createInfo.eAddressV)
        , m_eAddressW(createInfo.eAddressW)
        , m_eFilterMin(createInfo.eFilterMin)
        , m_eFilterMag(createInfo.eFilterMag)
        , m_eFilterMip(createInfo.eFilterMip)
        , m_eBorderColor(createInfo.eBorderColor)
        , m_fLodMinClamp(createInfo.fLodMinClamp)
        , m_fLodMaxClamp(createInfo.fLodMaxClamp)
        , m_fLodMipBias(createInfo.fLodMipBias)
        , m_eComparisonFunc(createInfo.eComparisonFunc)
        , m_nMaxAnisotropy(createInfo.nMaxAnisotropy)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanSampler::RHIVulkanSampler")

        createVkSampler();
    }

    RHIVulkanSampler::~RHIVulkanSampler()
    {
        Destroy();
    }

    void RHIVulkanSampler::Destroy()
    {
        if (m_vkSampler != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkSampler(m_vkSampler);
        }
        m_vkSampler = VK_NULL_HANDLE;
    }

    void RHIVulkanSampler::createVkSampler()
    {
        if (!m_pVulkanDevice->CreateVkSampler(m_eAddressU,
                                              m_eAddressV,
                                              m_eAddressW,
                                              m_eBorderColor,
                                              m_eFilterMin,
                                              m_eFilterMag,
                                              m_eFilterMip,
                                              m_eComparisonFunc,
                                              m_nMaxAnisotropy > 1,
                                              m_nMaxAnisotropy,
                                              m_fLodMinClamp,
                                              m_fLodMaxClamp,
                                              m_fLodMipBias,
                                              m_vkSampler))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanSampler::createVkSampler: CreateVkSampler failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_SAMPLER, reinterpret_cast<uint64_t>(m_vkSampler), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanSampler::createVkSampler: Create VkSampler success, Name: [%s] !", m_strDebugName.c_str());
        }
    }
    
}; //LostPeterPluginRHIVulkan