/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SCENE_OBJECT_H_
#define _VULKAN_SCENE_OBJECT_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanSceneObject 
    {
    public:
        VulkanSceneObject();
        virtual ~VulkanSceneObject();

    public:
        FMatrix4 matWorld;
        FMatrix4 matTexTransform;

        int numFramesDirty;
		uint32_t objCBIndex;

		VulkanMaterial* pMaterial;
		VulkanMesh* pMesh;

		uint32_t indexCount;
		uint32_t startIndexLocation;
		int baseVertexLocation;
    };

}; //LostPeter

#endif