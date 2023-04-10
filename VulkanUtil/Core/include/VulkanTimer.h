/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

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