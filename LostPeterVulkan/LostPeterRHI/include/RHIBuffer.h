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

#ifndef _RHI_BUFFER_H_
#define _RHI_BUFFER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIBuffer : public FNonCopyable
    {
    public:
        RHIBuffer(RHIDevice* pDevice, const RHIBufferCreateInfo& createInfo);
        virtual ~RHIBuffer();

    public:
    protected:
        RHIDevice* m_pDevice;

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }
        

    public:
        virtual void Destroy() = 0;

        virtual void* Map(RHIMapType eMap, uint32 nOffset, uint32 nLength) = 0;
        virtual void UnMap() = 0;
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewCreateInfo& createInfo) = 0;
    };

}; //LostPeterRHI

#endif