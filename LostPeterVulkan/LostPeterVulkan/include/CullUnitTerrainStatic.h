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

#ifndef _CULL_UNIT_TERRAIN_STATIC_H_
#define _CULL_UNIT_TERRAIN_STATIC_H_

#include "CullUnitTerrain.h"

namespace LostPeterVulkan
{
    class vulkanExport CullUnitTerrainStatic : public CullUnitTerrain
    {
    public:
        CullUnitTerrainStatic(const String& nameUnit);
        virtual ~CullUnitTerrainStatic();

    public:
        

    public:
        virtual void Destroy();

    };

}; //LostPeterVulkan

#endif