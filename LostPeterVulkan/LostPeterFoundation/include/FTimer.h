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

#ifndef _F_TIMER_H_
#define _F_TIMER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FTimer
    {
    public:
        FTimer();
        ~FTimer();

    public:
    protected:
        TimePoint m_tpStart;
        TimePoint m_tpLast;

    public:
        LP_FORCEINLINE const TimePoint& GetTimePointStart() const { return m_tpStart; }
        LP_FORCEINLINE const TimePoint& GetTimePointLast() const { return m_tpLast; }

    public:
        TimePoint Now();
        float SecondsBetween(TimePoint& a, TimePoint& b);

        float GetTimeSinceStart();
        float GetTimeDelta();

        void Tick();
    };

}; //LostPeterFoundation

#endif