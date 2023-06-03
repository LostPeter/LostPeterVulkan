/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPreInclude.h"
#include "../include/FTimer.h"

namespace LostPeterFoundation
{
    FTimer::FTimer()
    {
        m_tpStart = m_tpLast = Now();
    }

    FTimer::~FTimer()
    {
        
    }

    TimePoint FTimer::Now()
    {
        return std::chrono::steady_clock::now();
    }

    float FTimer::SecondsBetween(TimePoint& a, TimePoint& b)
    {
        return std::chrono::duration<float>(a - b).count();
    }

    float FTimer::GetTimeSinceStart()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpStart);
    }

    float FTimer::GetTimeDelta()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpLast);
    }

    void FTimer::Tick()
    {
        m_tpLast = Now();
    }

}; //LostPeterFoundation