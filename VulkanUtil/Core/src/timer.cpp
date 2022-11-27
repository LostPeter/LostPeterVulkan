#include "../include/preinclude.h"
#include "../include/timer.h"

namespace LibUtil
{
    Timer::Timer()
    {
        m_tpStart = m_tpLast = Now();
    }

    Timer::~Timer()
    {
        
    }

    TimePoint Timer::Now()
    {
        return std::chrono::steady_clock::now();
    }

    float Timer::SecondsBetween(TimePoint& a, TimePoint& b)
    {
        return std::chrono::duration<float>(a - b).count();
    }

    float Timer::GetTimeSinceStart()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpStart);
    }

    float Timer::GetTimeDelta()
    {
        TimePoint now = Now();
        return SecondsBetween(now, m_tpLast);
    }

    void Timer::Tick()
    {
        m_tpLast = Now();
    }

}; //LibUtil