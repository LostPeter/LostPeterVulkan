/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIShaderModule.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIShaderModule::RHIShaderModule(RHIDevice* pDevice, const RHIShaderModuleCreateInfo& createInfo)   
        : RHIObject(pDevice)
    {
        m_eObject = RHIObjectType::RHI_Object_ShaderModule;
    }

    RHIShaderModule::~RHIShaderModule()
    {
        
    }
    

}; //LostPeterRHI