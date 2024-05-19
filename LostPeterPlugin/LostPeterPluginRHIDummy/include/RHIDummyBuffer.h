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

#ifndef _RHI_DUMMY_BUFFER_H_
#define _RHI_DUMMY_BUFFER_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyBuffer : public RHIBuffer
    {
    public:
        RHIDummyBuffer(RHIDummyDevice* pDummyDevice, const RHIBufferCreateInfo& createInfo);
        virtual ~RHIDummyBuffer();

    public:
    protected:
        RHIDummyDevice* m_pDummyDevice;
        RHIDummyBufferView* m_pDummyBufferView;

        std::vector<uint8> m_aDummyData;

    public:
        F_FORCEINLINE RHIDummyDevice* GetDummyDevice() { return m_pDummyDevice; }
        F_FORCEINLINE RHIDummyBufferView* GetDummyBufferView() { return m_pDummyBufferView; }


    public:
        virtual void Destroy();
        virtual void DestroyBufferView();
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewCreateInfo& createInfo);

        virtual void* Map(RHIMapType eMap, uint32 nOffset, uint32 nLength);
        virtual void UnMap();
    };

}; //LostPeterPluginRHIDummy

#endif