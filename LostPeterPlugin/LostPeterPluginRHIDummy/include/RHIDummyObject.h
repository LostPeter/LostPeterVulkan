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

#ifndef _RHI_DUMMY_OBJECT_H_
#define _RHI_DUMMY_OBJECT_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyObject
    {
    public:
        RHIDummyObject(RHIDummyDevice* pDummyDevice);
        virtual ~RHIDummyObject();

    public:
    protected:
        RHIDummyDevice* m_pDummyDevice;

    public:
        F_FORCEINLINE RHIDummyDevice* GetDummyDevice() { return m_pDummyDevice; }

        
    public:
        
    };
    
}; //LostPeterPluginRHIDummy

#endif