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
        , matTransform(MathUtil::Identity4x4())
    {

    }

    VulkanMaterial::~VulkanMaterial()
    {
        
    }

}; //LostPeter