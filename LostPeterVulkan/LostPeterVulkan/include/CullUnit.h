/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_UNIT_H_
#define _CULL_UNIT_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullUnit : public Base
    {
    public:
        CullUnit(const String& nameUnit);
        virtual ~CullUnit();

    public:
        VulkanCullUnitType typeCullUnit;


    public:
        virtual void Destroy() = 0;
        virtual void Init() = 0;

    public:
        virtual bool IsCulling() = 0;
        virtual int GetRenderCount() = 0;  
        virtual ComputeBuffer* GetRenderArgsCB() = 0;

    };

}; //LostPeterVulkan

#endif