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

#ifndef _CULL_OBJECT_DYNAMIC_H_
#define _CULL_OBJECT_DYNAMIC_H_

#include "Base.h"
#include "CullUnitObject.h"

namespace LostPeterVulkan
{
    ////////////////////// CullObjectDynamic /////////////////////////
    class vulkanExport CullObjectDynamic : public FSingleton<CullObjectDynamic>
                                         , public Base
    {
    public:
        CullObjectDynamic();
        virtual ~CullObjectDynamic();

    public:
        ////////////////////// CullUnitObjectDynamic /////////////////////
        class vulkanExport CullUnitObjectDynamic : public CullUnitObject
        {
        public:
            CullUnitObjectDynamic(const String& nameUnit, CullObjectDynamic* pCullOD);
            virtual ~CullUnitObjectDynamic();

        public:
            CullObjectDynamic* pCullObjectDynamic;
            

        public:
            void Destroy();
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
        typedef std::vector<CullUnitObjectDynamic*> CullUnitObjectDynamicPtrVector;
        typedef std::map<String, CullUnitObjectDynamic*> CullUnitObjectDynamicPtrMap;


    public:
        static const String s_nameCullObjectDynamic;
        static const String s_nameCullUnitObjectDynamic;
        

    public:
        CullUnitObjectDynamic* pCullUnitObjectDynamic;

    public:
        static CullObjectDynamic& GetSingleton();
		static CullObjectDynamic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();
    };

}; //LostPeterVulkan

#endif