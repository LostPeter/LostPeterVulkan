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

#ifndef _F_REF_COUNT_H_
#define _F_REF_COUNT_H_

#include "FPreDefine.h"
#include "FAtomicCount.h"

namespace LostPeterFoundation
{
    class LPF_Export FRefCount
    {
    public:
        FRefCount();
        virtual ~FRefCount();

    public:
    protected:
        FAtomicCount m_refCounter;
        
    public:
        LP_FORCEINLINE int32 GetRef() { return m_refCounter.GetRefCount(); }

        LP_FORCEINLINE int32 AddRef() { return m_refCounter.AddRefCount(); }
        LP_FORCEINLINE int32 DelRef() { return m_refCounter.DelRefCount(); }
    };

}; //LostPeterFoundation

#endif