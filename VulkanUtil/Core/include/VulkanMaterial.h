// author: LostPeter
// time:   2022-12-22

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
        glm::vec3 fresnelR0;
        float roughness;
        float alpha;
        glm::mat4 matTransform;

    public:


    };

}; //LostPeter

#endif