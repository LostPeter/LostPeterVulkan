/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyShaderModule.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyShaderModule::RHIDummyShaderModule(RHIDummyDevice* pDummyDevice, const RHIShaderModuleCreateInfo& createInfo)
        : RHIShaderModule(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyShaderModule::RHIDummyShaderModule")
    }

    RHIDummyShaderModule::~RHIDummyShaderModule()
    {
        Destroy();
    }
    
    void RHIDummyShaderModule::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy