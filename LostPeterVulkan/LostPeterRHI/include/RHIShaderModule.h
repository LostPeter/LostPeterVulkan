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

#ifndef _RHI_SHADER_MODULE_H_
#define _RHI_SHADER_MODULE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIShaderModule : public RHIObject
    {
    public:
        RHIShaderModule(RHIDevice* pDevice, const RHIShaderModuleCreateInfo& createInfo);
        virtual ~RHIShaderModule();

    public:
    protected:


    public:
        

    };

}; //LostPeterRHI

#endif