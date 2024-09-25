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

#include "../include/CullRenderData.h"
#include "../include/VulkanWindow.h"
#include "../include/ComputeBuffer.h"

namespace LostPeterVulkan
{
    CullRenderData::CullRenderData()
        : nRenderIndex(0)
        , nClusterOffset(0)
        , nMaxMaterialCount(0)
        , pBuffer_Instance(nullptr) 
    {
        
    }

    CullRenderData::~CullRenderData()
    {
        Destroy();
    }

    void CullRenderData::Destroy()
    {
        Clear();
    }

    void CullRenderData::Init()
    {

    }

    void CullRenderData::Clear()
    {
        this->aLodDatas.clear();
        this->nRenderIndex = 0;
        this->nClusterOffset = 0;
        this->nMaxMaterialCount = 0;
        this->aInstanceDatas.clear();
        this->aClusterDatas.clear();
        F_DELETE(pBuffer_Instance)
    }
    
}; //LostPeterVulkan