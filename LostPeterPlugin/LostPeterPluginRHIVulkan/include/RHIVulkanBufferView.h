/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_BUFFER_VIEW_H_
#define _RHI_VULKAN_BUFFER_VIEW_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBufferView : public RHIBufferView
    {
    public:
        RHIVulkanBufferView(RHIVulkanBuffer* pBuffer, const RHIBufferViewCreateInfo& createInfo);
        virtual ~RHIVulkanBufferView();

    public:
    protected:
        RHIVulkanBuffer* m_pBuffer;
        uint32 m_nStride;
        uint32 m_nSize;
        uint32 m_nOffset;
        RHIIndexFormatType m_eIndexFormat;

    public:
        F_FORCEINLINE RHIVulkanBuffer* GetBuffer() { return m_pBuffer; }
        F_FORCEINLINE uint32 GetStride() { return m_nStride; }
        F_FORCEINLINE uint32 GetSize() { return m_nSize; }
        F_FORCEINLINE uint32 GetOffset() { return m_nOffset; }
        F_FORCEINLINE RHIIndexFormatType GetIndexFormatType() { return m_eIndexFormat; }

    public:
        virtual void Destroy();


    protected:
        

    };
    
}; //LostPeterPluginRHIVulkan

#endif