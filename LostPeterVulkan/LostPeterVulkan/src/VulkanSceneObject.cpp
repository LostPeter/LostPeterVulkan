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

#include "../include/PreInclude.h"
#include "../include/VulkanSceneObject.h"
#include "../include/VulkanSceneManager.h"
#include "../include/VulkanMaterial.h"

namespace LostPeter
{
    VulkanSceneObject::VulkanSceneObject()
        : matWorld(FMath::Identity4x4())
		, matTexTransform(FMath::Identity4x4())
		, numFramesDirty(VulkanSceneManager::s_numFrameResources)
		, objCBIndex(-1)
		, pMaterial(nullptr)
		, pMeshGeometry(nullptr)
		, indexCount(0)
		, startIndexLocation(0)
		, baseVertexLocation(0)
    {

    }

    VulkanSceneObject::~VulkanSceneObject()
    {
        
    }

}; //LostPeter