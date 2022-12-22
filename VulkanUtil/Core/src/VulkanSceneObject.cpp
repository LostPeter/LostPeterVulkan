#include "../include/PreInclude.h"
#include "../include/VulkanSceneObject.h"
#include "../include/VulkanSceneManager.h"
#include "../include/VulkanMaterial.h"

namespace LostPeter
{
    VulkanSceneObject::VulkanSceneObject()
        : matWorld(MathUtil::Identity4x4())
		, matTexTransform(MathUtil::Identity4x4())
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