/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_BIND_GROUP_H_
#define _RHI_BIND_GROUP_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIBindGroup : public RHIObject
    {
    public:
        RHIBindGroup(RHIDevice* pDevice, const RHIBindGroupCreateInfo& createInfo);
        virtual ~RHIBindGroup();

    public:
    protected:
        RHIBindGroupPool* m_pBindGroupPool;
        RHIBindGroupLayout* m_pBindGroupLayout;

    public:
        F_FORCEINLINE RHIBindGroupPool* GetBindGroupPool() const { return m_pBindGroupPool; }
        F_FORCEINLINE RHIBindGroupLayout* GetBindGroupLayout() const { return m_pBindGroupLayout; }


    public:

    };
    
}; //LostPeterRHI

#endif