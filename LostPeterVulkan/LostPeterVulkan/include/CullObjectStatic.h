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

#ifndef _CULL_OBJECT_STATIC_H_
#define _CULL_OBJECT_STATIC_H_

#include "Base.h"
#include "CullUnitObject.h"

namespace LostPeterVulkan
{
    ////////////////////// CullObjectStatic //////////////////////////
    class vulkanExport CullObjectStatic : public FSingleton<CullObjectStatic>
                                        , public Base
    {
    public:
        CullObjectStatic();
        virtual ~CullObjectStatic();

    public:
        ////////////////////// CullUnitObjectStatic //////////////////////
        class vulkanExport CullUnitObjectStatic : public CullUnitObject
        {
        public:
            CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS);
            virtual ~CullUnitObjectStatic();

        public:
            CullObjectStatic* pCullObjectStatic;

            int nLodCount;
            CullRenderDataPtrVector aCullRenderData;
            CullRenderDataPtr2IndexMap mapCullRenderData2Index;

            int nObjectCount;
            CullObjectConstantsVector aCullObjectConstants;

            int nRenderArgsCount;

            BufferCompute* pCB_CullObjects;
            BufferCompute* pCB_LodArgs;
            BufferCompute* pCB_RenderArgs;
            BufferCompute* pCB_Result;

            bool isRender;

        public:
            virtual void Destroy();
            virtual void Init();

        protected:
            void destroyComputeBuffers();
            void destroyDatas();
            
            void createComputeBuffers();

        public:
            virtual bool IsCulling();
            virtual int GetRenderCount();  
            virtual BufferCompute* GetRenderArgsCB();

        public:
            virtual int GetRenderDataCount();
            virtual CullRenderData* GetRenderData(int index);
            
            virtual int GetObjectDataCount();
            virtual CullObjectConstantsVector* GetObjectDatas();

            virtual int GetLodCount();

            virtual BufferCompute* GetObjectDataCB();
            virtual BufferCompute* GetLodArgsCB();
            virtual BufferCompute* GetResultCB();

            virtual void UpdateBuffer();

        public:
            bool HasCullRenderData(CullRenderData* pCullRenderData);
            void AddCullRenderData(CullRenderData* pCullRenderData);
            void RemoveCullRenderData(CullRenderData* pCullRenderData);
            void RefreshCullRenderData();
        };
        typedef std::vector<CullUnitObjectStatic*> CullUnitObjectStaticPtrVector;
        typedef std::map<String, CullUnitObjectStatic*> CullUnitObjectStaticPtrMap;

    public:
        static const String s_nameCullObjectStatic;
        static const String s_nameCullUnitObjectStatic;

        static float s_aLodMaxDistance[6];
        
        static int s_nRenderCountMax;
        static int s_nInstanceCountMax;

    public:
        CullUnitObjectStatic* pCullUnitObjectStatic;
        

    public:
        static CullObjectStatic& GetSingleton();
		static CullObjectStatic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();

    public: 
        CullRenderData* AddStaticCullRenderData(CullLodData* pCullLodData);
        bool AddStaticCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData);
        void RemoveStaticCullRenderData(CullRenderData* pCullRenderData);

    };

}; //LostPeterVulkan

#endif