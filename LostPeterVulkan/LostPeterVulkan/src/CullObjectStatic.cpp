/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/CullObjectStatic.h"
#include "../include/VulkanWindow.h"
#include "../include/CullManager.h"
#include "../include/BufferCompute.h"
#include "../include/CullLodData.h"
#include "../include/CullRenderData.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"

template<> LostPeterVulkan::CullObjectStatic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectStatic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    ////////////////////// CullUnitObjectStatic //////////////////////
    CullObjectStatic::CullUnitObjectStatic::CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS)
        : CullUnitObject(nameUnit)
        , pCullObjectStatic(pCullOS)
        , nLodCount((int)Vulkan_Lod_Count)
        , nObjectCount(0)
        , nRenderArgsCount(0)
        , pCB_CullObjects(nullptr)
        , pCB_LodArgs(nullptr)
        , pCB_RenderArgs(nullptr)
        , pCB_Result(nullptr)

        , isRender(true)
    {
        F_Assert(pCullObjectStatic != nullptr && "CullUnitObjectStatic::CullUnitObjectStatic")
    }

    CullObjectStatic::CullUnitObjectStatic::~CullUnitObjectStatic()
    {
        Destroy();
    }

    void CullObjectStatic::CullUnitObjectStatic::Destroy()
    {
        destroyComputeBuffers();
        destroyDatas();
    }
        void CullObjectStatic::CullUnitObjectStatic::destroyComputeBuffers()
        {
            F_DELETE(this->pCB_CullObjects)
            F_DELETE(this->pCB_LodArgs)
            F_DELETE(this->pCB_RenderArgs)
            F_DELETE(this->pCB_Result)
        }
        void CullObjectStatic::CullUnitObjectStatic::destroyDatas()
        {
            size_t count = this->aCullRenderData.size();
            for (size_t i = 0; i < count; i++)
            {
                CullRenderData* pRenderData = this->aCullRenderData[i];
                CullManager::GetSingleton().BackCullRenderData(pRenderData);
            }
            this->aCullRenderData.clear();
            this->mapCullRenderData2Index.clear();

            this->nObjectCount = 0;
            this->aCullObjectConstants.clear();

            this->nRenderArgsCount = 0;
        }       

    void CullObjectStatic::CullUnitObjectStatic::Init()
    {
        createComputeBuffers();

    }
        void CullObjectStatic::CullUnitObjectStatic::createComputeBuffers()
        {
            //pCB_CullObjects
            {
                this->pCB_CullObjects = new BufferCompute("BufferCompute-Static-CullObjects", CullObjectStatic::s_nInstanceCountMax, sizeof(CullObjectConstants));
            }
            //pCB_LodArgs
            {
                int count_lodArgs = this->nLodCount * CullObjectStatic::s_nRenderCountMax * 2;
                this->pCB_LodArgs = new BufferCompute("BufferCompute-Static-LodArgs", count_lodArgs, sizeof(float));
            }
            //pCB_RenderArgs
            {
                int count_renderArgs = this->nLodCount * CullObjectStatic::s_nRenderCountMax * 5;
                this->pCB_RenderArgs = new BufferCompute("BufferCompute-Static-RenderArgs", count_renderArgs, sizeof(uint));
            }
            //pCB_Result
            {
                int count_result = this->nLodCount * CullObjectStatic::s_nInstanceCountMax * 5;
                this->pCB_Result = new BufferCompute("BufferCompute-Static-Result", count_result, sizeof(uint));
            }
        }

    bool CullObjectStatic::CullUnitObjectStatic::IsCulling()
    {
        if (this->isRender)
        {
            return true;
        }
        return false;
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderCount()
    {
        return this->nRenderArgsCount;
    }

    BufferCompute* CullObjectStatic::CullUnitObjectStatic::GetRenderArgsCB()
    {
        return this->pCB_RenderArgs; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderDataCount()
    {
        return (int)this->aCullRenderData.size();
    }

    CullRenderData* CullObjectStatic::CullUnitObjectStatic::GetRenderData(int index)
    {
        if (index < 0 || index >= this->aCullRenderData.size())
            return nullptr;
        return this->aCullRenderData[index];
    }

    int CullObjectStatic::CullUnitObjectStatic::GetObjectDataCount()
    {
        return (int)this->aCullObjectConstants.size();
    }

    CullObjectConstantsVector* CullObjectStatic::CullUnitObjectStatic::GetObjectDatas()
    {
        return &this->aCullObjectConstants; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetLodCount()
    {
        return this->nLodCount;
    }

    BufferCompute* CullObjectStatic::CullUnitObjectStatic::GetObjectDataCB()
    {
        return this->pCB_CullObjects; 
    }

    BufferCompute* CullObjectStatic::CullUnitObjectStatic::GetLodArgsCB()
    {
        return this->pCB_LodArgs; 
    }

    BufferCompute* CullObjectStatic::CullUnitObjectStatic::GetResultCB()
    {
        return this->pCB_Result;
    }

    void CullObjectStatic::CullUnitObjectStatic::UpdateBuffer()
    {

    }

    bool CullObjectStatic::CullUnitObjectStatic::HasCullRenderData(CullRenderData* pCullRenderData)
    {
        if (pCullRenderData == nullptr)
            return false;
        CullRenderDataPtr2IndexMap::iterator itFind = this->mapCullRenderData2Index.find(pCullRenderData);
        return itFind != this->mapCullRenderData2Index.end();
    }
    void CullObjectStatic::CullUnitObjectStatic::AddCullRenderData(CullRenderData* pCullRenderData)
    {
        if (HasCullRenderData(pCullRenderData))
            return;
        CullLodData* pLodData = pCullRenderData->pCullLodData;
        int count_lod = this->nLodCount;
        int count_instance = (int)pLodData->aInstanceDatas.size();
        int count_material = (int)pLodData->aMaterialConstants.size();

        //Render
        int index = (int)this->aCullRenderData.size();
        this->aCullRenderData.push_back(pCullRenderData);
        this->mapCullRenderData2Index[pCullRenderData] = index;

        //Object
        for (int i = 0; i < count_instance; i++)
        {
            CullObjectInstanceConstants& instanceData = pLodData->aInstanceDatas[i];

            CullObjectConstants cullObject;
            MeshSub* pMeshSub = pLodData->pMesh->GetMeshSub(0);
            FVector3 vCenter = FMath::Transform(instanceData.mat4Object2World, pMeshSub->aabb.GetCenter());
            FVector3 vSize = FMath::Transform(instanceData.mat4Object2World, pMeshSub->aabb.GetHalfSize());
            cullObject.vPos = FVector4(vCenter.x, vCenter.y, vCenter.z, 1.0f);
            cullObject.vExt = FVector4(vSize.x, vSize.y, vSize.z, 1.0f);
            cullObject.nRenderIndex = (uint)pCullRenderData->nRenderIndex;
            cullObject.nRenderCount = (uint)pLodData->aMaterialConstants.size();
            cullObject.nObjectOffset = (uint)pCullRenderData->nObjectOffset;
            cullObject.nIsNoHizTest = 0;
            this->aCullObjectConstants.push_back(cullObject);
        }

        //Buffer
        float* pLodArgs = (float*)this->pCB_LodArgs->GetBuffer();
        uint* pRenderArgs = (uint*)this->pCB_RenderArgs->GetBuffer();
        uint* pResult = (uint*)this->pCB_Result->GetBuffer();
        for (int i = 0; i < count_lod; i++)
        {
             MeshSub* pMeshSub = pLodData->pMesh->GetMeshSub(i);

            int baseIndexLod = CullObjectStatic::s_nRenderCountMax * i * 2;
            int baseIndexRender = CullObjectStatic::s_nRenderCountMax * i * 5;
            int baseIndexResult = CullObjectStatic::s_nInstanceCountMax * i;
            int nIndexRenderArgs = pCullRenderData->nRenderIndex;

            for (int j = 0; j < count_material; j++)
            {
                pRenderArgs[baseIndexRender + nIndexRenderArgs * 5 + 0] = pMeshSub->poIndexCount;
                pRenderArgs[baseIndexRender + nIndexRenderArgs * 5 + 1] = (uint)count_instance;
                pRenderArgs[baseIndexRender + nIndexRenderArgs * 5 + 2] = 0;
                pRenderArgs[baseIndexRender + nIndexRenderArgs * 5 + 3] = 0;
                pRenderArgs[baseIndexRender + nIndexRenderArgs * 5 + 4] = 0;

                pLodArgs[baseIndexLod + nIndexRenderArgs * 2 + 0] = pLodData->fLodDistanceMin;
                pLodArgs[baseIndexLod + nIndexRenderArgs * 2 + 1] = pLodData->fLodDistanceMax;

                nIndexRenderArgs++;
            }

            int nIndexResult = pCullRenderData->nObjectOffset;
            for (int j = 0; j < count_instance; j++)
            {
                pResult[baseIndexResult + nIndexResult] = (uint)j;
                nIndexResult++;
            }
        }
        this->nObjectCount += count_instance;
        this->nRenderArgsCount += count_material;

        this->pCB_LodArgs->UpdateBuffer();
        this->pCB_RenderArgs->UpdateBuffer();
        this->pCB_Result->UpdateBuffer();
        this->pCB_CullObjects->UpdateBuffer(0, this->aCullObjectConstants.size() * sizeof(CullObjectConstants), this->aCullObjectConstants.data());
    }
    void CullObjectStatic::CullUnitObjectStatic::RemoveCullRenderData(CullRenderData* pCullRenderData)
    {
        CullRenderDataPtr2IndexMap::iterator itFindIndex = mapCullRenderData2Index.find(pCullRenderData);
        if (itFindIndex == mapCullRenderData2Index.end())
            return;
        mapCullRenderData2Index.erase(itFindIndex);

        CullRenderDataPtrVector::iterator itFind = std::find(aCullRenderData.begin(), aCullRenderData.end(), pCullRenderData);
        if (itFind != aCullRenderData.end())
            aCullRenderData.erase(itFind);
        
        CullObjectConstantsVector::iterator it = aCullObjectConstants.begin();
        CullObjectConstantsVector::iterator itEnd = aCullObjectConstants.end();
        CullObjectConstantsVector::iterator itNext = it;
        while (itNext != itEnd)
        {
            it = itNext;
            ++ itNext;
            if (it->nObjectOffset == pCullRenderData->nObjectOffset)
            {
                aCullObjectConstants.erase(it);
            }
        }

        CullManager::GetSingleton().BackCullRenderData(pCullRenderData);
    }
    void CullObjectStatic::CullUnitObjectStatic::RefreshCullRenderData()
    {
        this->nObjectCount = 0;
        this->nRenderArgsCount = 0;

        this->aCullObjectConstants.clear();
        size_t count_renderdata = this->aCullRenderData.size();
        for (size_t i = 0; i < count_renderdata; i++)
        {
            //Render
            CullRenderData* pCullRenderData = this->aCullRenderData[i];
            pCullRenderData->RefreshParam(this->nRenderArgsCount, this->nObjectCount);
            CullLodData* pLodData = pCullRenderData->pCullLodData;
            int count_lod = this->nLodCount;
            int nIndexRenderArgsCur = pCullRenderData->nRenderIndex;
            int nIndexResultCur = pCullRenderData->nObjectOffset;
            int count_instance = (int)pLodData->aInstanceDatas.size();
            int count_mat = (int)pLodData->aMaterialConstants.size();

            //Object
            for (int j = 0; j < count_instance; j++)
            {
                CullObjectInstanceConstants& instanceData = pLodData->aInstanceDatas[j];

                CullObjectConstants cullObject;
                MeshSub* pMeshSub = pLodData->pMesh->GetMeshSub(0);
                FVector3 vCenter = FMath::Transform(instanceData.mat4Object2World, pMeshSub->aabb.GetCenter());
                FVector3 vSize = FMath::Transform(instanceData.mat4Object2World, pMeshSub->aabb.GetHalfSize());
                cullObject.vPos = FVector4(vCenter.x, vCenter.y, vCenter.z, 1.0f);
                cullObject.vExt = FVector4(vSize.x, vSize.y, vSize.z, 1.0f);
                cullObject.nRenderIndex = (uint)pCullRenderData->nRenderIndex;
                cullObject.nRenderCount = (uint)pLodData->aMaterialConstants.size();
                cullObject.nObjectOffset = (uint)pCullRenderData->nObjectOffset;
                cullObject.nIsNoHizTest = 0;
                this->aCullObjectConstants.push_back(cullObject);
            }

            //Buffer
            float* pLodArgs = (float*)this->pCB_LodArgs->GetBuffer();
            uint* pRenderArgs = (uint*)this->pCB_RenderArgs->GetBuffer();
            uint* pResult = (uint*)this->pCB_Result->GetBuffer();
            for (int p = 0; p < count_lod; p++)
            {
                MeshSub* pMeshSub = pLodData->pMesh->GetMeshSub(p);

                nIndexRenderArgsCur = pCullRenderData->nRenderIndex;
                nIndexResultCur = pCullRenderData->nObjectOffset;
                int baseIndexLod = CullObjectStatic::s_nRenderCountMax * p * 2;
                int baseIndexRender = CullObjectStatic::s_nRenderCountMax * p * 5;
                int baseIndexResult = CullObjectStatic::s_nRenderCountMax * p;
               
                for (int q = 0; q < count_mat; q++)
                {
                    pRenderArgs[baseIndexRender + nIndexRenderArgsCur * 5 + 0] = pMeshSub->poIndexCount;
                    pRenderArgs[baseIndexRender + nIndexRenderArgsCur * 5 + 1] = (uint)count_instance;
                    pRenderArgs[baseIndexRender + nIndexRenderArgsCur * 5 + 2] = 0;
                    pRenderArgs[baseIndexRender + nIndexRenderArgsCur * 5 + 3] = 0;
                    pRenderArgs[baseIndexRender + nIndexRenderArgsCur * 5 + 4] = 0;

                    pLodArgs[baseIndexLod + nIndexRenderArgsCur * 2 + 0] = pLodData->fLodDistanceMin;
                    pLodArgs[baseIndexLod + nIndexRenderArgsCur * 2 + 1] = pLodData->fLodDistanceMax;

                    nIndexRenderArgsCur++;
                }

                for (int q = 0; q < count_instance; q++)
                {
                    pResult[baseIndexResult + nIndexResultCur] = (uint)q;
                    nIndexResultCur++;
                }
            }
            this->nObjectCount += count_instance;
            this->nRenderArgsCount += count_mat;
        }

        this->pCB_LodArgs->UpdateBuffer();
        this->pCB_RenderArgs->UpdateBuffer();
        this->pCB_Result->UpdateBuffer();
        this->pCB_CullObjects->UpdateBuffer(0, this->aCullObjectConstants.size() * sizeof(CullObjectConstants), this->aCullObjectConstants.data());
    }


    ////////////////////// CullObjectStatic //////////////////////////
    CullObjectStatic* CullObjectStatic::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	CullObjectStatic& CullObjectStatic::GetSingleton()
	{  
		F_Assert(ms_Singleton && "CullObjectStatic::GetSingleton")
		return (*ms_Singleton);     
	}

    const String CullObjectStatic::s_nameCullObjectStatic = "CullObjectStatic";
    const String CullObjectStatic::s_nameCullUnitObjectStatic = "CullUnitObjectStatic";
    float CullObjectStatic::s_aLodMaxDistance[6] = 
    {
          0.0f,   128.0f,
		128.0f,   256.0f,
		256.0f,   512.0f
    };
    int CullObjectStatic::s_nRenderCountMax = 200;
    int CullObjectStatic::s_nInstanceCountMax = 100000;
    
    CullObjectStatic::CullObjectStatic()
        : Base(s_nameCullObjectStatic)
        , pCullUnitObjectStatic(nullptr)
        
    {
        
    }

    CullObjectStatic::~CullObjectStatic()
    {
        Destroy();
    }

    void CullObjectStatic::Destroy()
    {
        F_DELETE(this->pCullUnitObjectStatic)
        
    }
    
    void CullObjectStatic::Init()
    {
        this->pCullUnitObjectStatic = new CullUnitObjectStatic(s_nameCullUnitObjectStatic, this);
        this->pCullUnitObjectStatic->Init();
    }

    CullRenderData* CullObjectStatic::AddStaticCullRenderData(CullLodData* pLodData)
    {
        if (pLodData == nullptr)
            return nullptr;

        CullLodDataPtrVector aLodDatas;
        aLodDatas.push_back(pLodData);
        CullRenderDataPtrVector aCullRenderData;
        if (AddStaticCullRenderDatas(aLodDatas, aCullRenderData))
        {
            if (aCullRenderData.size() > 0)
                return aCullRenderData[0];
        }
        return nullptr;
    }
    bool CullObjectStatic::AddStaticCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData)
    {
        int count_lod = (int)aLodDatas.size();
        if (count_lod <= 0)
        {
            F_LogError("*********************** CullObjectStatic::AddStaticCullRenderDatas: Lod data count <= 0 !");
            return false;
        }

        int count_render = 0;
        int count_instance = 0;
        for (int i = 0; i < count_lod; i++)
        {
            CullLodData* pLodData = aLodDatas[i];
            if (pLodData->pMesh == nullptr)
            {
                F_LogError("*********************** CullObjectStatic::AddStaticCullRenderDatas: Mesh is null !");
                return false;
            }
            int countMeshSub = pLodData->pMesh->GetMeshSubCount();
            if (countMeshSub != this->pCullUnitObjectStatic->nLodCount)
            {
                F_LogError("*********************** CullObjectStatic::AddStaticCullRenderDatas: Lod data count is wrong, src: [%d], dst: [%d] !", countMeshSub, this->pCullUnitObjectStatic->nLodCount);
                return false;
            }
            count_render += countMeshSub;
            count_instance += (int)pLodData->aInstanceDatas.size();
        }

        if (this->pCullUnitObjectStatic->nRenderArgsCount + count_render >= CullObjectStatic::s_nRenderCountMax)
        {
            F_LogError("*********************** CullObjectStatic::AddStaticCullRenderDatas: Render count is >= [%d] !", CullObjectStatic::s_nRenderCountMax);
            return false;
        }
        if (this->pCullUnitObjectStatic->nObjectCount + count_instance >= CullObjectStatic::s_nInstanceCountMax)
        {
            F_LogError("*********************** CullObjectStatic::AddStaticCullRenderDatas: Render count is >= [%d] !", CullObjectStatic::s_nRenderCountMax);
            return false;
        }

        for (int i = 0; i < count_lod; i++)
        {
            CullLodData* pLodData = aLodDatas[i];

            CullRenderData* pCullRenderData = CullManager::GetSingleton().GetCullRenderData();
            pCullRenderData->Destroy();
            pCullRenderData->Init(pLodData, this->pCullUnitObjectStatic->nRenderArgsCount, this->pCullUnitObjectStatic->nObjectCount, CullObjectStatic::s_nInstanceCountMax);
            this->pCullUnitObjectStatic->AddCullRenderData(pCullRenderData);

            aCullRenderData.push_back(pCullRenderData);
        }
        return true;
    }
    void CullObjectStatic::RemoveStaticCullRenderData(CullRenderData* pCullRenderData)
    {
        if (pCullRenderData == nullptr)
            return;
        
        this->pCullUnitObjectStatic->RemoveCullRenderData(pCullRenderData);
        this->pCullUnitObjectStatic->RefreshCullRenderData();
    }

}; //LostPeterVulkan