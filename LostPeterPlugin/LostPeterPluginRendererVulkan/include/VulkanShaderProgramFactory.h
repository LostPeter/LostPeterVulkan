/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-10
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SHADER_PROGRAM_FACTORY_H_
#define _VULKAN_SHADER_PROGRAM_FACTORY_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderProgramFactory : public ShaderProgramFactory
    {
    public:
        VulkanShaderProgramFactory(VulkanDevice* pDevice);
        virtual ~VulkanShaderProgramFactory();

    public:
        static const String m_strShaderLanguage;

    public:
    protected:
        VulkanDevice* m_pDevice;

    public:
        virtual const String& GetShaderLanguage() const;

    public:
        virtual ShaderProgram* Create(ResourceManager* pResourceManager, 
                                      uint32 nGroup, 
                                      const String& strName, 
                                      const String& strGroupName, 
                                      ResourceHandle nHandle,
                                      bool bIsManualLoad,
                                      ResourceManualLoader* pResourceManualLoader);
    };

}; //LostPeterPluginRendererVulkan

#endif