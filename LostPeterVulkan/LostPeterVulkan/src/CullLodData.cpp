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
#include "../include/Mesh.h"

namespace LostPeterVulkan
{
    CullLodData::CullLodData()
        : isRender(true)
        , isCastShadow(false)
        , fLodDistanceMin(10.0f)
        , fLodDistanceMax(50.0f)

        , pMesh(nullptr)
    {

    }

    CullLodData::~CullLodData()
    {
        Clear();
    }

    void CullLodData::Clear()
    {
        this->isRender = true;
        this->pMesh = nullptr;
        this->aMaterialConstants.clear();
        this->aInstanceDatas.clear();
    }
    
}; //LostPeterVulkan