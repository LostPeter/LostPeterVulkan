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

#ifndef _VULKAN_SHADER_PROGRAM_H_
#define _VULKAN_SHADER_PROGRAM_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderProgram : public ShaderProgram
    {
        friend class VulkanShaderManager;

    public:
        VulkanShaderProgram(VulkanDevice* pDevice,
                            ResourceManager* pResourceManager,
                            uint32 nGroup, 
                            const String& strName,
                            const String& strGroupName,
                            ResourceHandle nHandle,
                            bool bIsManualLoad = false,
                            ResourceManualLoader* pResourceManualLoader = nullptr);
        ~VulkanShaderProgram();

    public:
    protected:
        VulkanDevice* m_pDevice;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        virtual void Destroy();

     public:
        

    protected:

       
    ////Resource
    protected:
        virtual void addParameterInherit();


    ////ShaderProgram
    public:


    protected:
		virtual void destroyInternalResourcesImpl();

        virtual void createInternalResourcesImpl();
    };

}; //LostPeterPluginRendererVulkan

#endif