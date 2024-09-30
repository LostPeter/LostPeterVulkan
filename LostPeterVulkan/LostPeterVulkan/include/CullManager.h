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

#ifndef _CULL_MANAGER_H_
#define _CULL_MANAGER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullManager : public FSingleton<CullManager>
                                   , public Base
    {
    public:
        CullManager();
        virtual ~CullManager();

    public:
        static int s_nRenderCount_Init;
        static int s_nRenderCount_Max;
        static int s_nRenderCount_Step;
        static int s_nInstanceCount_Init;
        static int s_nInstanceCount_Max;
        static int s_nInstanceCount_Step;

        ObjectPool<CullLodData>* pCullLodDataPool;
        ObjectPool<CullRenderData>* pCullRenderDataPool;
        ObjectPool<CullObjectConstants>* pCullObjectConstantsPool;
        ObjectPool<CullObjectInstanceConstants>* pCullObjectInstanceConstantsPool;

    public:
        VKPipelineComputeCull* pVKPipelineComputeCull;

        CullUnitPtrVector aCullUnits;
        CullUnitObjectPtrVector aCullUnitObjects;
        CullUnitTerrainPtrVector aCullUnitTerrains;

        CullObjectStatic* pCullObjectStatic;
        CullObjectDynamic* pCullObjectDynamic;

        bool isEnable;

    public:
        static CullManager& GetSingleton();
		static CullManager* GetSingletonPtr();

    public:
        F_FORCEINLINE bool IsEnable() const { return isEnable; }

    public:
        void Destroy();
        void Init(VKPipelineComputeCull* pPipelineComputeCull);

        void RefreshEnable();

        void ExecuteHizCullTest(VkCommandBuffer& commandBuffer);
        void ExecuteHizDepthGenerate(VkCommandBuffer& commandBuffer);

    protected:  
        void destroyCullObjects();
        void destroyPools();

        void createPools();
        void createCullObjects();

    public:
        void AddCullUnit(CullUnit* pCullUnit);
        void RemoveCullUnit(CullUnit* pCullUnit);
        void RemoveAllCullUnit();

    public:
        CullLodData* GetCullLodData();
        void BackCullLodData(CullLodData* pCullLodData);
        void BackCullLodDatas(const CullLodDataPtrVector& aLodDatas);

        CullRenderData* GetCullRenderData();
        void BackCullRenderData(CullRenderData* pCullRenderData);
        void BackCullLodDatas(const CullRenderDataPtrVector& aCullRenderDatas);

        CullObjectConstants* GetCullObjectConstants();
        void BackCullObjectConstants(CullObjectConstants* pCullObject);

        CullObjectInstanceConstants* GetCullObjectInstanceConstants();
        void BackCullObjectInstanceConstants(CullObjectInstanceConstants* pCullObjectInstance);

    public:
        //Object Static
        CullRenderData* AddStaticCullRenderData(CullLodData* pCullLodData);
        bool AddStaticCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData);
        void RemoveStaticCullRenderData(CullRenderData* pCullRenderData);

        //Object Dynamic
        CullRenderData* AddDynamicCullRenderData(CullLodData* pCullLodData);
        bool AddDynamicCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData);
        void RemoveDynamicCullRenderData(CullRenderData* pCullRenderData);
    };

}; //LostPeterVulkan

#endif