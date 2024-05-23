/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_SHADER_MODULE_CACHE_H_
#define _RHI_DUMMY_SHADER_MODULE_CACHE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyShaderModuleCache : public RHIShaderModuleCache
                                                   , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyShaderModuleCache(RHIDummyDevice* pDummyDevice, const RHIShaderModuleCacheCreateInfo& createInfo);
    public:
        virtual ~RHIDummyShaderModuleCache();

    public:
    protected:


    public:
        

    public:
        virtual void Destroy();

    };

}; //LostPeterPluginRHIDummy

#endif