/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ObjectCamera.h"
#include "../include/Viewport.h"
#include "../include/SceneManager.h"
#include "../include/RenderEngine.h"

namespace LostPeterEngine
{
    const String ObjectCamera::ms_strMovableType = "Movable_Camera";

    ObjectCamera::ObjectCamera(const String& nameCamera, SceneManager* pSceneManager)
        : ObjectFrustum(nameCamera, pSceneManager)
        , m_vPosLocal(FMath::ms_v3Zero)
		, m_qRotLocal(FMath::ms_qUnit)
        , m_vPosWorld(FMath::ms_v3Zero)
		, m_qRotWorld(FMath::ms_qUnit)
        , m_vPosReal(FMath::ms_v3Zero)
		, m_qRotReal(FMath::ms_qUnit)
        , m_pViewport(nullptr)
    {
        this->m_typeObject = Vulkan_Object_Camera;
    }

    ObjectCamera::~ObjectCamera()
    {

    }

    void ObjectCamera::SetDirectionLocal(const FVector3& vDirLocal)
    {

    }   


    void ObjectCamera::RenderScene(Viewport* pViewport, bool bIncludeOverlays)
    {

    }


    const String& ObjectCamera::GetMovableType() const
    {
        return ms_strMovableType;
    }

	float ObjectCamera::GetBoundingRadius() const
    {
        return 0.0f;
    }

}; //LostPeterEngine