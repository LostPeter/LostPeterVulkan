/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_UNIT_OBJECT_DYNAMIC_H_
#define _CULL_UNIT_OBJECT_DYNAMIC_H_

#include "CullUnitObject.h"

namespace LostPeterVulkan
{
    class vulkanExport CullUnitObjectDynamic : public CullUnitObject
    {
    public:
        CullUnitObjectDynamic(const String& nameUnit);
        virtual ~CullUnitObjectDynamic();

    public:
        

    public:
        void Destroy();

    };

}; //LostPeterVulkan

#endif