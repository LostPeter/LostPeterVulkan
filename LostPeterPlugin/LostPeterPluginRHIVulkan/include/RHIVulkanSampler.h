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

#ifndef _RHI_VULKAN_SAMPLER_H_
#define _RHI_VULKAN_SAMPLER_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSampler : public RHISampler
                                           , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanSampler(RHIVulkanDevice* pVulkanDevice, const RHISamplerCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanSampler();

    public:
    protected:
        VkSampler m_vkSampler;

        RHIAddressType m_eAddressU;
        RHIAddressType m_eAddressV;
        RHIAddressType m_eAddressW;
        RHIFilterType m_eFilterMin;
        RHIFilterType m_eFilterMag;
        RHIFilterType m_eFilterMip;
        RHIBorderColorType m_eBorderColor;
        float m_fLodMinClamp;
        float m_fLodMaxClamp;
        float m_fLodMipBias;
        RHIComparisonFuncType m_eComparisonFunc;
        uint8 m_nMaxAnisotropy;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkSampler& GetVkSampler() { return m_vkSampler; }
        F_FORCEINLINE VkSampler* GetVkSamplerPtr() { return &m_vkSampler; }

        F_FORCEINLINE RHIAddressType GetAddressU() const { return m_eAddressU; }
        F_FORCEINLINE RHIAddressType GetAddressV() const { return m_eAddressV; }
        F_FORCEINLINE RHIAddressType GetAddressW() const { return m_eAddressW; }
        F_FORCEINLINE RHIFilterType GetFilterMin() const { return m_eFilterMin; }
        F_FORCEINLINE RHIFilterType GetFilterMag() const { return m_eFilterMag; }
        F_FORCEINLINE RHIFilterType GetFilterMip() const { return m_eFilterMip; }
        F_FORCEINLINE RHIBorderColorType GetBorderColor() const { return m_eBorderColor; }
        F_FORCEINLINE float GetLodMinClamp() const { return m_fLodMinClamp; }
        F_FORCEINLINE float GetLodMaxClamp() const { return m_fLodMaxClamp; }
        F_FORCEINLINE float GetLodMipBias() const { return m_fLodMipBias; }
        F_FORCEINLINE RHIComparisonFuncType GetComparisonFunc() const { return m_eComparisonFunc; }
        F_FORCEINLINE uint8 GetMaxAnisotropy() const { return m_nMaxAnisotropy; }
        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkSampler();
    };

}; //LostPeterPluginRHIVulkan

#endif