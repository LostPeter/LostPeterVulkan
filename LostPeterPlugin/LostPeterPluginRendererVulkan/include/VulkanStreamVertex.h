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

#ifndef _VULKAN_STREAM_VERTEX_H_
#define _VULKAN_STREAM_VERTEX_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanStreamVertex : public StreamVertex
    {
    public:
        VulkanStreamVertex(VulkanDevice* pDevice, uint32 nTarget, 
                           size_t nSizeInBytes, EStreamUsageType eStreamUsage,
					       bool bSystemMemory, bool bUseShadowStream);
        virtual ~VulkanStreamVertex();

    public:
    private:
        VulkanDevice* m_pDevice;
        uint32 m_nTarget;

        VkBuffer m_vkBuffer;
        VkDeviceMemory m_vkDeviceMemory;
        
    public:
        E_FORCEINLINE VkBuffer GetVkBuffer() const { return m_vkBuffer; }
        E_FORCEINLINE VkDeviceMemory GetVkDeviceMemory() const { return m_vkDeviceMemory; }

    public:
        virtual void ReadData(size_t nOffset, size_t nLength, void* pDest);
		virtual void WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream = false);

		virtual void CopyData(Stream& streamSrc, size_t nSrcOffset, size_t nDstOffset, size_t nLength, bool bDiscardWholeStream = false);

        virtual void UpdateFromShadow();

    protected:
		virtual void* lockImpl(size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
		virtual void unlockImpl();

    private:
        void destroyVkBuffer();
        void createVkBuffer();
    };

}; //LostPeter

#endif