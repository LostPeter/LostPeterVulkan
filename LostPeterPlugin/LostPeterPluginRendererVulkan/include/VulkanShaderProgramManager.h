/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SHADER_PROGRAM_MANAGER_H_
#define _VULKAN_SHADER_PROGRAM_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderProgramManager : public ShaderProgramManager 
    {
    public:
        VulkanShaderProgramManager(VulkanDevice* pDevice);
        virtual ~VulkanShaderProgramManager();

    public:
    protected:
        VulkanDevice* m_pDevice;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:

    
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