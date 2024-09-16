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

#ifndef _CULL_UNIT_OBJECT_DYNAMIC_H_
#define _CULL_UNIT_OBJECT_DYNAMIC_H_

#include "CullUnitObject.h"

namespace LostPeterVulkan
{
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

}; //LostPeterVulkan

#endif