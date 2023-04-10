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

#ifndef _VULKAN_MATERIAL_H_
#define _VULKAN_MATERIAL_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanMaterial
    {
    public:
        VulkanMaterial();
        ~VulkanMaterial();

    public:
        std::string name;
        int matCBIndex;
        int diffuseSrvHeapIndex;
        int normalSrvHeapIndex;
        int numFramesDirty;

        glm::vec4 factorAmbient;
        glm::vec4 factorDiffuse;
        glm::vec4 factorSpecular;

        float shininess;
        float alpha;
        float lighting;
        float reserve0;

        TextureConstants aTexLayers[MAX_TEXTURE_COUNT];

    public:


    };

}; //LostPeter

#endif