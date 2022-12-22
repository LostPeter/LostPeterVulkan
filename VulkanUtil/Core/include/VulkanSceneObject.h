// author: LostPeter
// time:   2022-10-30

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
        glm::mat4 matWorld;
        glm::mat4 matTexTransform;

        int numFramesDirty;
		uint32_t objCBIndex;

		VulkanMaterial* pMaterial;
		MeshGeometry* pMeshGeometry;

		uint32_t indexCount;
		uint32_t startIndexLocation;
		int baseVertexLocation;
    };

}; //LostPeter

#endif