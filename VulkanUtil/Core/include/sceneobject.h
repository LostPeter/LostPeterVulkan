// author: LostPeter
// time:   2022-10-30

#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport SceneObject 
    {
    public:
        SceneObject();
        virtual ~SceneObject();

    public:
        glm::mat4 matWorld;
        glm::mat4 matTexTransform;

        int numFramesDirty;
		uint32_t objCBIndex;

		Material* pMaterial;
		MeshGeometry* pMeshGeometry;

		uint32_t indexCount;
		uint32_t startIndexLocation;
		int baseVertexLocation;
    };

}; //LibUtil

#endif