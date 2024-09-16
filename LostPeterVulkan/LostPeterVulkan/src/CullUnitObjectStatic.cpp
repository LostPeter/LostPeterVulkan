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

#include "../include/CullUnitObjectStatic.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    float CullUnitObjectStatic::s_aLodMaxDistance[6] = 
    {
          0.0f,   128.0f,
		128.0f,   256.0f,
		256.0f,   512.0f
    };
    int CullUnitObjectStatic::s_nMaxRenderCount = 200;
    int CullUnitObjectStatic::s_nStepRenderCount = 20;
    int CullUnitObjectStatic::s_nMaxInstanceCount = 100000;
    
    CullUnitObjectStatic::CullUnitObjectStatic(const String& nameUnit)
        : CullUnitObject(nameUnit)
    {

    }

    CullUnitObjectStatic::~CullUnitObjectStatic()
    {

    }

    void CullUnitObjectStatic::Destroy()
    {
        
    }
    
}; //LostPeterVulkan