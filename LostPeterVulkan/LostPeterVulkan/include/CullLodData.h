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

#ifndef _CULL_LOD_DATA_H_
#define _CULL_LOD_DATA_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullLodData
    {
    public:
        CullLodData();
        ~CullLodData();

    public:
        bool isRender;
        bool isCastShadow;
        float fLodDistanceMin;
        float fLodDistanceMax;
        
        Mesh* pMesh;
        MaterialConstantsVector aMaterialConstants;
        CullObjectInstanceConstantsVector aInstanceDatas;

    public:
        void Clear();

    };

}; //LostPeterVulkan

#endif