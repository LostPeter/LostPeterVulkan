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

#include "../include/CullLodData.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"

namespace LostPeterVulkan
{
    CullLodData::CullLodData(const String& nameLodData)
        : Base(nameLodData)

        , isRender(true)
        , isCastShadow(false)
        , fLodDistanceMin(10.0f)
        , fLodDistanceMax(50.0f)
        
        , pMesh(nullptr)

    {

    }

    CullLodData::~CullLodData()
    {

    }

    
    
}; //LostPeterVulkan