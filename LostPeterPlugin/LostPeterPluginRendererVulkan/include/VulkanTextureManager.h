/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_TEXTURE_MANAGER_H_
#define _VULKAN_TEXTURE_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanTextureManager : public TextureManager
    {
    public:
        VulkanTextureManager(VulkanDevice* pDevice, bool bCanRestrictImageViewUsage);
        virtual ~VulkanTextureManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        bool m_bCanRestrictImageViewUsage;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE bool CanRestrictImageViewUsage() const { return m_bCanRestrictImageViewUsage; }

    public:
        virtual FPixelFormatType GetPixelFormatNative(FTextureType eTexture, FPixelFormatType ePixelFormat, int32 nUsage);

        virtual bool CheckSupport(FPixelFormatType ePixelFormat, uint32 nTextureFlags) const;

    protected:
        virtual Resource* createImpl(uint32 nGroup,
									 const String& strName,
			                         const String& strGroupName,
									 ResourceHandle nHandle, 
                                     bool bIsManualLoad,
                                     ResourceManualLoader* pManualLoader, 
			                         const NameValuePairMap* pLoadParams);
    };

}; //LostPeterPluginRendererVulkan

#endif