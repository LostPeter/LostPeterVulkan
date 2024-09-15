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

#ifndef _CULL_UNIT_MANAGER_H_
#define _CULL_UNIT_MANAGER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullUnitManager : public Base
    {
    public:
        CullUnitManager();
        virtual ~CullUnitManager();

    public:
        static int s_nMaxLodCount;

    public:
        


    public:
        void Destroy();

    };

}; //LostPeterVulkan

#endif