/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanMaterial.h"

namespace LostPeter
{
    VulkanMaterial::VulkanMaterial()
        : name("")
        , matCBIndex(-1)
        , diffuseSrvHeapIndex(-1)
        , normalSrvHeapIndex(-1)
        , numFramesDirty(0)
        , factorAmbient(1.0f, 1.0f, 1.0f, 1.0f)
        , factorDiffuse(1.0f, 1.0f, 1.0f, 1.0f)
        , factorSpecular(1.0f, 1.0f, 1.0f, 1.0f)
        , shininess(0.25f)
        , alpha(1.0f)
        , lighting(1.0f)
        , reserve0(0)
    {

    }

    VulkanMaterial::~VulkanMaterial()
    {
        
    }

}; //LostPeter