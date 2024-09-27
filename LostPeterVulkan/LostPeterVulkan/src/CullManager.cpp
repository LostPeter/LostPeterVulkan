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
#include "../include/CullRenderData.h"
#include "../include/CullUnit.h"
#include "../include/CullUnitObject.h"
#include "../include/CullUnitTerrain.h"
#include "../include/CullObjectStatic.h"
#include "../include/CullObjectDynamic.h"
#include "../include/VKPipelineComputeCull.h"

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
    int CullManager::s_nRenderCount_Max = 200;
    int CullManager::s_nRenderCount_Step = 20;
    int CullManager::s_nInstanceCount_Init = 1000;
    int CullManager::s_nInstanceCount_Max = 100000;
    int CullManager::s_nInstanceCount_Step = 100;

    CullManager::CullManager()
        : Base("CullManager")
        , pVKPipelineComputeCull(nullptr)

        , pCullRenderDataPool(nullptr)
        , pCullObjectConstantsPool(nullptr)
        , pCullObjectInstanceConstantsPool(nullptr)

        , pCullObjectStatic(nullptr)
        , pCullObjectDynamic(nullptr)

        , isInit(false)
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
            F_DELETE(pCullRenderDataPool)
            F_DELETE(pCullObjectConstantsPool)
            F_DELETE(pCullObjectInstanceConstantsPool)
        }

    void CullManager::Init(VKPipelineComputeCull* pPipelineComputeCull)
    {
        this->pVKPipelineComputeCull = pPipelineComputeCull;

        createPools();
        createCullObjects();
    }
        void CullManager::createPools()
        {
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

    CullRenderData* CullManager::GetCullRenderData()
    {
        return pCullRenderDataPool->Get();
    }
    void CullManager::BackCullRenderData(CullRenderData* pCullRenderData)
    {
        pCullRenderData->Destroy();
        pCullRenderDataPool->Back(pCullRenderData);
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

}; //LostPeterVulkan