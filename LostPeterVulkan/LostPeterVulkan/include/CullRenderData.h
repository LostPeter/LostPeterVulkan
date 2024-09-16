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
    class vulkanExport CullRenderData : public Base
    {
    public:
        CullRenderData(const String& nameRenderData);
        virtual ~CullRenderData();

    public:
        CullLodDataPtrVector aLodDatas;
        int nRenderIndex;
        int nClusterOffset;
        int nMaxMaterialCount;
        InstanceDataVector aInstanceDatas;
        CullObjectConstantsVector aClusterDatas;



    public:
        void Destroy();
        void Init();

        void Clear();

    public:


    };

}; //LostPeterVulkan

#endif