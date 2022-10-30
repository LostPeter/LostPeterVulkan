#include "../include/preinclude.h"
#include "../include/sceneobject.h"
#include "../include/scenemanager.h"

namespace LibUtil
{
    SceneObject::SceneObject()
        : matWorld(MathUtil::Identity4x4())
		, matTexTransform(MathUtil::Identity4x4())
		, numFramesDirty(SceneManager::s_numFrameResources)
		, objCBIndex(-1)
		, pMaterial(nullptr)
		, pMeshGeometry(nullptr)
		, indexCount(0)
		, startIndexLocation(0)
		, baseVertexLocation(0)
    {

    }

    SceneObject::~SceneObject()
    {
        
    }

}; //LibUtil