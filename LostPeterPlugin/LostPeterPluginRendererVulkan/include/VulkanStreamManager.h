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


    public:
        //StreamVertex
        virtual StreamVertex* CreateStreamVertex(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false);
		virtual StreamVertex* CreateStreamVertex(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false);

        //StreamIndex
        virtual StreamIndex* CreateStreamIndex(EStreamIndexType eStreamIndex, uint32 nIndexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false);

        //StreamUniform
		virtual StreamUniform* CreateStreamUniform(uint32 nUniformSize, uint32 nUniformCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false);
		virtual StreamUniform* CreateStreamUniform(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false);

    };

}; //LostPeterPluginRendererVulkan

#endif