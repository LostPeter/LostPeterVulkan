/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_BIND_GROUP_POOL_H_
#define _RHI_BIND_GROUP_POOL_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIBindGroupPool : public FNonCopyable
    {
    public:
        RHIBindGroupPool(RHIDevice* pDevice, const RHIBindGroupPoolCreateInfo& createInfo);
        virtual ~RHIBindGroupPool();

    public:
    protected:
        RHIDevice* m_pDevice;

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }
        

    public:
        virtual void Destroy() = 0;

    };
    
}; //LostPeterRHI

#endif