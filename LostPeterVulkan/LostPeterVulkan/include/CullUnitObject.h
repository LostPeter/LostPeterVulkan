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
        virtual CullRenderData* GetRenderData(int index) = 0;
        
        virtual int GetObjectDataCount() = 0;
        virtual CullObjectConstantsVector* GetObjectDatas() = 0;

        virtual int GetLodCount() = 0;

        virtual BufferCompute* GetObjectDataCB() = 0;
        virtual BufferCompute* GetLodArgsCB() = 0;

        virtual BufferCompute* GetResultCB() = 0;
        virtual BufferCompute* GetClipCB() { return nullptr; }

        virtual void UpdateBuffer() = 0;
    };

}; //LostPeterVulkan

#endif