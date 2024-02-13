/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_STREAM_INDEX_H_
#define _VULKAN_STREAM_INDEX_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanStreamIndex : public StreamIndex
    {
    public:
        VulkanStreamIndex(VulkanDevice* pDevice, 
                          uint32 nTarget, 
                          EStreamIndexType eStreamIndex, 
                          uint32 nIndexCount, 
                          EStreamUsageType eStreamUsage,
					      bool bIsUseSystemMemory, 
                          bool bIsUseShadowStream);
        virtual ~VulkanStreamIndex();

    public:
    private:
        VulkanDevice* m_pDevice;
        uint32 m_nTarget;

        VkBuffer m_vkBuffer;
        VkDeviceMemory m_vkDeviceMemory;
        
    public:
        F_FORCEINLINE VkBuffer GetVkBuffer() const { return m_vkBuffer; }
        F_FORCEINLINE VkDeviceMemory GetVkDeviceMemory() const { return m_vkDeviceMemory; }

    public:
        virtual void ReadData(uint32 nOffset, uint32 nLength, void* pDest);
		virtual void WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream = false);

		virtual void CopyData(Stream& streamSrc, uint32 nSrcOffset, uint32 nDstOffset, uint32 nLength, bool bIsDiscardWholeStream = false);

        virtual void UpdateFromShadow();

    protected:
		virtual void* lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock);
		virtual void unlockImpl();

    private:
        void destroyVkBuffer();
        void createVkBuffer();
    };

}; //LostPeterPluginRendererVulkan

#endif