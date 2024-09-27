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

#ifndef _CULL_RENDER_DATA_H_
#define _CULL_RENDER_DATA_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullRenderData
    {
    public:
        CullRenderData();
        virtual ~CullRenderData();

    public:
        CullLodDataPtrVector aLodDatas;
        int nRenderIndex;
        int nObjectOffset;
        int nMaxMaterialCount;
        
        CullObjectConstantsPtrVector aObjectDatas;
        CullObjectInstanceConstantsPtrVector aInstanceDatas;

        ComputeBuffer* pBuffer_Instance;

    public:
        void Destroy();
        void Init();

        void Clear();

    public:
        

    };

}; //LostPeterVulkan

#endif