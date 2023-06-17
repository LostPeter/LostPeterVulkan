/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_ATOMIC_COUNT_H_
#define _F_ATOMIC_COUNT_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FAtomicCount
    {
    public:
        FAtomicCount();
        FAtomicCount(int32 value);
        FAtomicCount(const FAtomicCount& other);
        ~FAtomicCount();

    public:
    protected:
        volatile int32 m_nCounter;
        
    public:
        int32 GetRefCount();
        int32 SetRefCount(int32 value);

        int32 AddRefCount();
        int32 AddRefCount(int32 value);

        int32 DelRefCount();
        int32 DelRefCount(int32 value);

        int32 Reset();
    };

}; //LostPeterFoundation

#endif