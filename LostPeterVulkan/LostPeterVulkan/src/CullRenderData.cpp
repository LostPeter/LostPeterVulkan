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
#include "../include/CullUnit.h"
#include "../include/CullLodData.h"
#include "../include/BufferCompute.h"
#include "../include/BufferUniform.h"
#include "../include/Mesh.h"

namespace LostPeterVulkan
{
    CullRenderData::CullRenderData()
        : pCullUnit(nullptr)
        , pDescriptorSets_ShadowMapDepthCull(nullptr)

        , nRenderIndex(0)
        , nObjectOffset(0)
        , nMaxMaterialCount(0)

        , pCullLodData(nullptr)
        , pBuffer_CullObjectInstances(nullptr) 
        , pBuffer_CullInstance(nullptr) 
    {
        
    }

    CullRenderData::~CullRenderData()
    {
        Destroy();
    }

    void CullRenderData::Destroy()
    {
        this->pCullUnit = nullptr;
        this->pDescriptorSets_ShadowMapDepthCull = nullptr;

        this->nRenderIndex = 0;
        this->nObjectOffset = 0;
        this->nMaxMaterialCount = 0;

        this->pCullLodData = nullptr;

        F_DELETE(pBuffer_CullObjectInstances)
        F_DELETE(pBuffer_CullInstance)
    }

    void CullRenderData::Init(CullLodData* pCLD, 
                              int renderIndex, 
                              int objectOffset, 
                              int objectMax,
                              CullUnit* pUnit)
    {
        RefreshNew(pCLD, renderIndex, objectOffset, objectMax, pUnit);
    }   

    void CullRenderData::RefreshNew(CullLodData* pCLD, int renderIndex, int objectOffset, int objectMax, CullUnit* pUnit)
    {
        if (pCLD != this->pCullLodData)
        {
            Destroy();
        }
        this->pCullUnit = pUnit;
        this->pCullLodData = pCLD;
        RefreshParam(renderIndex, objectOffset);
        RefreshCullObjectInstances();
        RefreshCullInstance();
    }

    void CullRenderData::RefreshParam(int renderIndex, int objectOffset)
    {
        this->nRenderIndex = renderIndex;
        this->nObjectOffset = objectOffset;
    }

    void CullRenderData::RefreshCullObjectInstances()
    {
        int count_instance = (int)this->pCullLodData->aInstanceDatas.size();
        if (this->pBuffer_CullObjectInstances == nullptr)
        {
            String nameCB = "BufferCompute-CullObjectInstances-" + this->pCullLodData->pMesh->GetName();
            this->pBuffer_CullObjectInstances = new BufferCompute(nameCB, count_instance, sizeof(CullObjectInstanceConstants));
        }
        this->pBuffer_CullObjectInstances->UpdateBuffer(0, count_instance * sizeof(CullObjectInstanceConstants), &this->pCullLodData->aInstanceDatas[0]);
    }

    void CullRenderData::RefreshCullInstance()
    {
        this->cullInstance.nObjectOffset = this->nObjectOffset;
        if (this->pBuffer_CullInstance == nullptr)
        {
            String nameCB = "BufferUniform-CullInstance-" + this->pCullLodData->pMesh->GetName();
            this->pBuffer_CullInstance = new BufferUniform(nameCB, 1, sizeof(CullInstanceConstants));
        }
        this->pBuffer_CullInstance->UpdateBuffer(0, sizeof(CullInstanceConstants), &this->cullInstance);
    }
    
}; //LostPeterVulkan