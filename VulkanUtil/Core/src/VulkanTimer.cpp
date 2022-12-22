#include "../include/PreInclude.h"
#include "../include/VulkanTimer.h"

namespace LostPeter
{
    VulkanTimer::VulkanTimer()
    {
        m_tpStart = m_tpLast = Now();
    }

    VulkanTimer::~VulkanTimer()
    {
        
    }

    TimePoint VulkanTimer::Now()
    {
        return std::chrono::steady_clock::now();
    }

    float VulkanTimer::SecondsBetween(TimePoint& a, TimePoint& b)
    {
        return std::chrono::duration<float>(a - b).count();
    }

    float VulkanTimer::GetTimeSinceStart()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpStart);
    }

    float VulkanTimer::GetTimeDelta()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpLast);
    }

    void VulkanTimer::Tick()
    {
        m_tpLast = Now();
    }

}; //LostPeter