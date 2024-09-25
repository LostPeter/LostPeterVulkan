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

            
        public:
            virtual void Destroy();
            virtual void Init();

        public:
            virtual bool IsCulling();
            virtual int GetRenderCount();  
            virtual ComputeBuffer* GetRenderArgsCB();

        public:
            virtual int GetRenderDataCount();
            virtual CullRenderData* GetRenderData();
            
            virtual int GetClusterDataCount(int index);
            virtual CullObjectConstantsVector* GetClusterDatas();

            virtual int GetLodCount();

            virtual ComputeBuffer* GetClusterDataCB();
            virtual ComputeBuffer* GetLodCB();

            virtual ComputeBuffer* GetResultCB();
            virtual ComputeBuffer* GetClipCB();

            virtual void UpdateBuffer();

        };
        typedef std::vector<CullUnitObjectStatic*> CullUnitObjectStaticPtrVector;
        typedef std::map<String, CullUnitObjectStatic*> CullUnitObjectStaticPtrMap;

    public:
        static const String s_nameCullObjectStatic;
        static const String s_nameCullUnitObjectStatic;

        static float s_aLodMaxDistance[6];
        static int s_nMaxRenderCount;
        static int s_nStepRenderCount;
        static int s_nMaxInstanceCount;

    public:
        CullUnitObjectStatic* pCullUnitObjectStatic;
        ObjectPool<CullRenderData*>* pCullRenderDataPool; 

    public:
        static CullObjectStatic& GetSingleton();
		static CullObjectStatic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();
    
    };

}; //LostPeterVulkan

#endif