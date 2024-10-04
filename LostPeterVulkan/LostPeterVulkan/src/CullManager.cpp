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

#include "../include/CullManager.h"
#include "../include/VulkanWindow.h"
#include "../include/CullLodData.h"
#include "../include/CullRenderData.h"
#include "../include/CullUnit.h"
#include "../include/CullUnitObject.h"
#include "../include/CullUnitTerrain.h"
#include "../include/CullObjectStatic.h"
#include "../include/CullObjectDynamic.h"
#include "../include/VKPipelineComputeCull.h"
#include "../include/BufferCompute.h"
#include "../include/BufferIndirectCommand.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"

template<> LostPeterVulkan::CullManager* LostPeterFoundation::FSingleton<LostPeterVulkan::CullManager>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    CullManager* CullManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	CullManager& CullManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "CullManager::GetSingleton")
		return (*ms_Singleton);     
	}

    int CullManager::s_nRenderCount_Init = 20;
    int CullManager::s_nRenderCount_Max = 1000;
    int CullManager::s_nRenderCount_Step = 20;
    int CullManager::s_nInstanceCount_Init = 1000;
    int CullManager::s_nInstanceCount_Max = 100000;
    int CullManager::s_nInstanceCount_Step = 100;

    CullManager::CullManager()
        : Base("CullManager")
        , pVKPipelineComputeCull(nullptr)

        , pCullLodDataPool(nullptr)
        , pCullRenderDataPool(nullptr)
        , pCullObjectConstantsPool(nullptr)
        , pCullObjectInstanceConstantsPool(nullptr)

        , pCullObjectStatic(nullptr)
        , pCullObjectDynamic(nullptr)

        , isEnable(false)
    {

    }
    CullManager::~CullManager()
    {
        Destroy();
    }

    void CullManager::Destroy()
    {
        RemoveAllCullUnit();
        destroyCullObjects();
        destroyPools();
    }
        void CullManager::destroyCullObjects()
        {
            F_DELETE(this->pCullObjectStatic)
            F_DELETE(this->pCullObjectDynamic)
        }
        void CullManager::destroyPools()
        {
            F_DELETE(pCullLodDataPool)
            F_DELETE(pCullRenderDataPool)
            F_DELETE(pCullObjectConstantsPool)
            F_DELETE(pCullObjectInstanceConstantsPool)
        }

    void CullManager::Init(VKPipelineComputeCull* pPipelineComputeCull)
    {
        this->pVKPipelineComputeCull = pPipelineComputeCull;

        createPools();
        createCullObjects();

        RefreshEnable();
    }
        void CullManager::createPools()
        {
            this->pCullLodDataPool = new ObjectPool<CullLodData>();
            this->pCullLodDataPool->stepCount = s_nRenderCount_Step;
            this->pCullLodDataPool->Reserve(s_nRenderCount_Init);
            
            this->pCullRenderDataPool = new ObjectPool<CullRenderData>();
            this->pCullRenderDataPool->stepCount = s_nRenderCount_Step;
            this->pCullRenderDataPool->Reserve(s_nRenderCount_Init);

            this->pCullObjectConstantsPool = new ObjectPool<CullObjectConstants>();
            this->pCullObjectConstantsPool->stepCount = s_nInstanceCount_Step;
            this->pCullObjectConstantsPool->Reserve(s_nInstanceCount_Init);

            this->pCullObjectInstanceConstantsPool = new ObjectPool<CullObjectInstanceConstants>();
            this->pCullObjectInstanceConstantsPool->stepCount = s_nInstanceCount_Step;
            this->pCullObjectInstanceConstantsPool->Reserve(s_nInstanceCount_Init);
        }
        void CullManager::createCullObjects()
        {
            //CullObjectStatic
            this->pCullObjectStatic = new CullObjectStatic();
            this->pCullObjectStatic->Init();
            AddCullUnit(this->pCullObjectStatic->pCullUnitObjectStatic);
            //CullObjectDynamic
            this->pCullObjectDynamic = new CullObjectDynamic();
            this->pCullObjectDynamic->Init();
            AddCullUnit(this->pCullObjectDynamic->pCullUnitObjectDynamic);
        }

    void CullManager::RefreshEnable()
    {
        this->isEnable = false;
        if (this->pVKPipelineComputeCull != nullptr && 
            (Base::GetWindowPtr()->isComputeCullFrustum || 
            Base::GetWindowPtr()->isComputeCullFrustumHizDepth))
        {
            this->isEnable = true;
        }
    }

    void CullManager::ExecuteHizCullTest(VkCommandBuffer& commandBuffer)
    {
        if (!this->isEnable)
            return;

        int count_unit = (int)this->aCullUnits.size();
        if (count_unit <= 0)
            return;
        VulkanWindow* pVulkanWindow = Base::GetWindowPtr();
        this->pVKPipelineComputeCull->UpdateBuffer_Cull();

        //Unit Object
        int count_unit_object = (int)this->aCullUnitObjects.size();
        for (int i = 0; i < count_unit_object; i++)
        {
            CullUnitObject* pUnitObject = this->aCullUnitObjects[i];
            if (!pUnitObject->IsCulling())
                continue;
            int count_render = pUnitObject->GetRenderCount();
            if (count_render <= 0)
                continue;

            int count_object = pUnitObject->GetObjectDataCount();
            int count_lod = pUnitObject->GetLodCount();
            if (count_object <= 0 || count_lod <= 0)
                continue;
            pUnitObject->UpdateBuffer();

            //Clear
            BufferIndirectCommand* pCB_RenderArgs = pUnitObject->GetRenderArgsCB();
            this->pVKPipelineComputeCull->UpdateDescriptorSet_CullClearArgs(pCB_RenderArgs);
            pVulkanWindow->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->pVKPipelineComputeCull->poPipeline_CullClearArgs);
            pVulkanWindow->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->pVKPipelineComputeCull->poPipelineLayout_CullClearArgs, 0, 1, &this->pVKPipelineComputeCull->poDescriptorSet_CullClearArgs, 0, 0);
            int x = FMath::CeilI(count_render / 64.0f);
            pVulkanWindow->dispatch(commandBuffer, x, 1, 1);

            //Test
            BufferCompute* pCB_CullObjects = pUnitObject->GetObjectDataCB();
            BufferCompute* pCB_LodArgs = pUnitObject->GetLodArgsCB();
            BufferCompute* pCB_Result = pUnitObject->GetResultCB();
            if (pVulkanWindow->isComputeCullFrustumHizDepth)
            {
                BufferCompute* pCB_Clip = pUnitObject->GetClipCB();
                if (pCB_Clip != nullptr)
                {   
                    this->pVKPipelineComputeCull->UpdateDescriptorSet_CullFrustumDepthHizClip(pCB_CullObjects, pCB_RenderArgs, pCB_LodArgs, pCB_Result, pCB_Clip);
                }
                else
                {
                    this->pVKPipelineComputeCull->UpdateDescriptorSet_CullFrustumDepthHiz(pCB_CullObjects, pCB_RenderArgs, pCB_LodArgs, pCB_Result);
                }
            }
            else if (pVulkanWindow->isComputeCullFrustum)
            {
                this->pVKPipelineComputeCull->UpdateDescriptorSet_CullFrustum(pCB_CullObjects, pCB_RenderArgs, pCB_LodArgs, pCB_Result);
            }
            x = FMath::CeilI(count_object / 64.0f); 
            pVulkanWindow->dispatch(commandBuffer, x, 1, 1);
        }

        //Unit Terrain
        int count_unit_terrain = (int)this->aCullUnitTerrains.size();
        for (int i = 0; i < count_unit_terrain; i++)
        {
            CullUnitTerrain* pUnitTerrain = this->aCullUnitTerrains[i];
            bool isCulling = pUnitTerrain->IsCulling();
            int count_render = pUnitTerrain->GetRenderCount();
            if (count_render <= 0)
                continue;

            //Clear


            //Test

        }
    }
    void CullManager::ExecuteHizDepthGenerate(VkCommandBuffer& commandBuffer)
    {
        
    }

    void CullManager::ExecuteShadowMapDraw(VkCommandBuffer& commandBuffer)
    {
        if (!this->isEnable)
            return;

        int count_unit = (int)this->aCullUnits.size();
        if (count_unit <= 0)
            return;
        VulkanWindow* pVulkanWindow = Base::GetWindowPtr();

        //Unit Object
        int index = 0;
        int count_unit_object = (int)this->aCullUnitObjects.size();
        for (int i = 0; i < count_unit_object; i++)
        {
            CullUnitObject* pUnitObject = this->aCullUnitObjects[i];
            if (!pUnitObject->IsCulling())
                continue;
            int count_render = pUnitObject->GetRenderCount();
            if (count_render <= 0)
                continue;
            int count_lod = pUnitObject->GetLodCount();

            BufferIndirectCommand* pRenderArg = pUnitObject->GetRenderArgsCB();
            BufferCompute* pResult = pUnitObject->GetResultCB();

            for (int j = 0; j < count_render; j++)
            {
                CullRenderData* pRenderData = pUnitObject->GetRenderData(j);
                CullLodData* pLodData = pRenderData->pCullLodData;
                int count_material = (int)pLodData->aMaterialConstants.size();
                if (!pLodData->isCastShadow)
                {
                    index += count_lod * count_material;
                    continue;
                }
                
                pVulkanWindow->Draw_Graphics_BindDescriptorSet_ShadowMapDepthCull(commandBuffer, pRenderData->pDescriptorSets_ShadowMapDepthCull);
                pVulkanWindow->Draw_Graphics_CullInstance_DepthShadowMapCullUnit(commandBuffer,
                                                                                 pRenderArg->poBuffer_IndirectCommand,
                                                                                 index,
                                                                                 pLodData->pMesh->GetMeshSub(0));

                index += count_lod * count_material;
            }
        }

        //Unit Terrain
        int count_unit_terrain = (int)this->aCullUnitTerrains.size();
        for (int i = 0; i < count_unit_terrain; i++)
        {
            CullUnitTerrain* pUnitTerrain = this->aCullUnitTerrains[i];
            bool isCulling = pUnitTerrain->IsCulling();
            int count_render = pUnitTerrain->GetRenderCount();
            if (count_render <= 0)
                continue;

            //Clear


            //Test

        }
    }

    void CullManager::AddCullUnit(CullUnit* pCullUnit)
    {
        F_Assert(pCullUnit != nullptr && "CullManager::AddCullUnit")

        CullUnitPtrVector::iterator itFind = std::find(this->aCullUnits.begin(), this->aCullUnits.end(), pCullUnit);
        if (itFind != this->aCullUnits.end())
            return;

        this->aCullUnits.push_back(pCullUnit);
        if (pCullUnit->typeCullUnit == Vulkan_CullUnit_Object)
        {
            CullUnitObject* pCullUnitObject = (CullUnitObject*)pCullUnit;
            this->aCullUnitObjects.push_back(pCullUnitObject);
        }
        else if (pCullUnit->typeCullUnit == Vulkan_CullUnit_Terrain)
        {
            CullUnitTerrain* pCullUnitTerrain = (CullUnitTerrain*)pCullUnit;
            this->aCullUnitTerrains.push_back(pCullUnitTerrain);
        }
    }
    void CullManager::RemoveCullUnit(CullUnit* pCullUnit)
    {
        F_Assert(pCullUnit != nullptr && "CullManager::RemoveCullUnit")

        CullUnitPtrVector::iterator itFind = std::find(this->aCullUnits.begin(), this->aCullUnits.end(), pCullUnit);
        if (itFind == this->aCullUnits.end())
            return;
        this->aCullUnits.erase(itFind);
        if (pCullUnit->typeCullUnit == Vulkan_CullUnit_Object)
        {
            CullUnitObject* pCullUnitObject = (CullUnitObject*)pCullUnit;
            CullUnitObjectPtrVector::iterator itFindObject = std::find(this->aCullUnitObjects.begin(), this->aCullUnitObjects.end(), pCullUnitObject);
            if (itFindObject == this->aCullUnitObjects.end())
                this->aCullUnitObjects.erase(itFindObject);
        }
        else if (pCullUnit->typeCullUnit == Vulkan_CullUnit_Terrain)
        {
            CullUnitTerrain* pCullUnitTerrain = (CullUnitTerrain*)pCullUnit;
            CullUnitTerrainPtrVector::iterator itFindTerrain = std::find(this->aCullUnitTerrains.begin(), this->aCullUnitTerrains.end(), pCullUnitTerrain);
            if (itFindTerrain == this->aCullUnitTerrains.end())
                this->aCullUnitTerrains.erase(itFindTerrain);
        }
    }
    void CullManager::RemoveAllCullUnit()
    {
        this->aCullUnits.clear();
        this->aCullUnitObjects.clear();
        this->aCullUnitTerrains.clear();
    }

    CullLodData* CullManager::GetCullLodData()
    {
        return pCullLodDataPool->Get();
    }
    void CullManager::BackCullLodData(CullLodData* pCullLodData)
    {
        pCullLodData->Clear();
        pCullLodDataPool->Back(pCullLodData);
    }   
    void CullManager::BackCullLodDatas(const CullLodDataPtrVector& aLodDatas)
    {
        size_t count = aLodDatas.size();
        for (size_t i = 0; i < count; i++)
        {
            CullLodData* pCullLodData = aLodDatas[i];
            BackCullLodData(pCullLodData);
        }
    }

    CullRenderData* CullManager::GetCullRenderData()
    {
        return pCullRenderDataPool->Get();
    }
    void CullManager::BackCullRenderData(CullRenderData* pCullRenderData)
    {
        pCullRenderData->Destroy();
        pCullRenderDataPool->Back(pCullRenderData);
    }
    void CullManager::BackCullLodDatas(const CullRenderDataPtrVector& aCullRenderDatas)
    {
        size_t count = aCullRenderDatas.size();
        for (size_t i = 0; i < count; i++)
        {
            CullRenderData* pCullRenderData = aCullRenderDatas[i];
            BackCullRenderData(pCullRenderData);
        }
    }

    CullObjectConstants* CullManager::GetCullObjectConstants()
    {
        return pCullObjectConstantsPool->Get();
    }
    void CullManager::BackCullObjectConstants(CullObjectConstants* pCullObject)
    {
        pCullObjectConstantsPool->Back(pCullObject);
    }

    CullObjectInstanceConstants* CullManager::GetCullObjectInstanceConstants()
    {
        return pCullObjectInstanceConstantsPool->Get();
    }
    void CullManager::BackCullObjectInstanceConstants(CullObjectInstanceConstants* pCullObjectInstance)
    {
        pCullObjectInstanceConstantsPool->Back(pCullObjectInstance);
    }

    //Object Static
    CullRenderData* CullManager::AddStaticCullRenderData(CullLodData* pCullLodData)
    {
        return this->pCullObjectStatic->AddStaticCullRenderData(pCullLodData);
    }
    bool CullManager::AddStaticCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData)
    {
        return this->pCullObjectStatic->AddStaticCullRenderDatas(aLodDatas, aCullRenderData);
    }
    void CullManager::RemoveStaticCullRenderData(CullRenderData* pCullRenderData)
    {
        this->pCullObjectStatic->RemoveStaticCullRenderData(pCullRenderData);
    }   

    //Object Dynamic
    CullRenderData* CullManager::AddDynamicCullRenderData(CullLodData* pCullLodData)
    {
        return this->pCullObjectDynamic->AddDynamicCullRenderData(pCullLodData);
    }
    bool CullManager::AddDynamicCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData)
    {
        return this->pCullObjectDynamic->AddDynamicCullRenderDatas(aLodDatas, aCullRenderData);
    }
    void CullManager::RemoveDynamicCullRenderData(CullRenderData* pCullRenderData)
    {   
        this->pCullObjectDynamic->RemoveDynamicCullRenderData(pCullRenderData);
    }   

}; //LostPeterVulkan