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

#ifndef _RHI_RENDER_PASS_H_
#define _RHI_RENDER_PASS_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIRenderPass : public FNonCopyable
    {
    public:
        RHIRenderPass(RHIDevice* pDevice, const RHIRenderPassCreateInfo& createInfo);
        virtual ~RHIRenderPass();

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