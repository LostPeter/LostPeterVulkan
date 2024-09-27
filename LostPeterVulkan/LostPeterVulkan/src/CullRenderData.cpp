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
#include "../include/CullManager.h"
#include "../include/ComputeBuffer.h"

namespace LostPeterVulkan
{
    CullRenderData::CullRenderData()
        : nRenderIndex(0)
        , nObjectOffset(0)
        , nMaxMaterialCount(0)

        , pCullLodData(nullptr)

        , pBuffer_Instance(nullptr) 
    {
        
    }

    CullRenderData::~CullRenderData()
    {
        Destroy();
    }

    void CullRenderData::Destroy()
    {
        this->nRenderIndex = 0;
        this->nObjectOffset = 0;
        this->nMaxMaterialCount = 0;

        if (this->pCullLodData != nullptr)
        {
             CullManager::GetSingleton().BackCullLodData(this->pCullLodData);
        }
        this->pCullLodData = nullptr;

        F_DELETE(pBuffer_Instance)
    }

    void CullRenderData::Init(CullLodData* pCLD, int renderIndex, int objectOffset, int objectMax)
    {
        RefreshNew(pCLD, renderIndex, objectOffset, objectMax);
    }   

    void CullRenderData::RefreshNew(CullLodData* pCLD, int renderIndex, int objectOffset, int objectMax)
    {
        if (pCLD != this->pCullLodData)
        {
            Destroy();
        }
        this->pCullLodData = pCLD;
        RefreshParam(renderIndex, objectOffset);
        RefreshInstance();
    }

    void CullRenderData::RefreshParam(int renderIndex, int objectOffset)
    {
        this->nRenderIndex = renderIndex;
        this->nObjectOffset = objectOffset;
    }

    void CullRenderData::RefreshInstance()
    {

    }
    
}; //LostPeterVulkan