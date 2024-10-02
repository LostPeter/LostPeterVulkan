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
        ~CullRenderData();

    public:
        int nRenderIndex;
        int nObjectOffset;
        int nMaxMaterialCount;

        CullLodData* pCullLodData;
        BufferCompute* pBuffer_CullObjectInstances;

        CullInstanceConstants cullInstance;
        BufferUniform* pBuffer_CullInstance;

    public:
        void Destroy();
        void Init(CullLodData* pCLD, int renderIndex, int objectOffset, int objectMax);

    public:
        void RefreshNew(CullLodData* pCLD, int renderIndex, int objectOffset, int objectMax);
        void RefreshParam(int renderIndex, int objectOffset);
        void RefreshCullObjectInstances();
        void RefreshCullInstance();
    };

}; //LostPeterVulkan

#endif