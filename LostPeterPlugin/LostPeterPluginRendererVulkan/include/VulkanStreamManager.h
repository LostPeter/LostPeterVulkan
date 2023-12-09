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

#ifndef _VULKAN_STREAM_MANAGER_H_
#define _VULKAN_STREAM_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
   class VulkanStreamManager : public StreamManager
    {
    public:
        VulkanStreamManager(VulkanDevice* pDevice);
        virtual ~VulkanStreamManager();

    public:
    private:
        VulkanDevice* m_pDevice;

    ////StreamManager
    public:
        virtual StreamVertex* CreateStreamVertex(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream = false);
		virtual StreamVertex* CreateStreamVertex(size_t nSizeInBytes, EStreamUsageType eStreamUsage, bool bUseShadowStream = false);

        virtual StreamIndex* CreateStreamIndex(EStreamIndexType eStreamIndex, size_t nIndexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream = false);

    
    };

}; //LostPeterPluginRendererVulkan

#endif