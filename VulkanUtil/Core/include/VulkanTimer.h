// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_TIMER_H_
#define _VULKAN_TIMER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanTimer
    {
    public:
        VulkanTimer();
        ~VulkanTimer();

    public:
    protected:
        TimePoint m_tpStart;
        TimePoint m_tpLast;

    public:
        const TimePoint& GetTimePointStart() const { return m_tpStart; }
        const TimePoint& GetTimePointLast() const { return m_tpLast; }

    public:
        TimePoint Now();
        float SecondsBetween(TimePoint& a, TimePoint& b);

        float GetTimeSinceStart();
        float GetTimeDelta();

        void Tick();
    };

}; //LostPeter

#endif