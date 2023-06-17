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

#include "../include/FAtomicCount.h"
#include "../include/FUtil.h"

namespace LostPeterFoundation
{
    FAtomicCount::FAtomicCount()
        : m_nCounter(0)
    {

    }

    FAtomicCount::FAtomicCount(int32 value)
        : m_nCounter(value)
    {

    }

    FAtomicCount::FAtomicCount(const FAtomicCount& other)
        : m_nCounter(other.m_nCounter)
    {

    }

    FAtomicCount::~FAtomicCount()
    {
        
    }

    int32 FAtomicCount::GetRefCount()
    {
        return FUtil::AtomicRead(&(const_cast<FAtomicCount*>(this)->m_nCounter));
    }

    int32 FAtomicCount::SetRefCount(int32 value)
    {
        return FUtil::InterlockedExchange(&m_nCounter, value);
    }

    int32 FAtomicCount::AddRefCount()
    {
        return FUtil::InterlockedIncrement(&m_nCounter);
    }

    int32 FAtomicCount::AddRefCount(int32 value)
    {
        return FUtil::InterlockedAdd(&m_nCounter, value);
    }

    int32 FAtomicCount::DelRefCount()
    {       
        return FUtil::InterlockedDecrement(&m_nCounter);
    }

    int32 FAtomicCount::DelRefCount(int32 value)
    {
        return FUtil::InterlockedAdd(&m_nCounter, -value);
    }

    int32 FAtomicCount::Reset()
    {
        return FUtil::InterlockedExchange(&m_nCounter, 0);
    }
    
}; //LostPeterFoundation