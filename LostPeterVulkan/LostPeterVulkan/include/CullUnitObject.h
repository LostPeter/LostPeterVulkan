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

#ifndef _CULL_UNIT_OBJECT_H_
#define _CULL_UNIT_OBJECT_H_

#include "CullUnit.h"

namespace LostPeterVulkan
{
    class vulkanExport CullUnitObject : public CullUnit
    {
    public:
        CullUnitObject(const String& nameUnit);
        virtual ~CullUnitObject();

    public:
        

    public:
        virtual int GetRenderDataCount() = 0;
        virtual CullRenderData* GetRenderData() = 0;
        
        virtual int GetClusterDataCount(int index) = 0;
        virtual const CullObjectConstantsVector& GetClusterDatas() = 0;

        virtual int GetLodCount() = 0;

        virtual ComputeBuffer* GetClusterDataCB() = 0;
        virtual ComputeBuffer* GetLodCB() = 0;

        virtual ComputeBuffer* GetResultCB() = 0;
        virtual ComputeBuffer* GetClipCB() { return nullptr; }

        virtual void UpdateBuffer() = 0;
    };

}; //LostPeterVulkan

#endif