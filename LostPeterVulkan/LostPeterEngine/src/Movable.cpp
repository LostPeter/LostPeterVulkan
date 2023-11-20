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

#include "../include/Movable.h"
#include "../include/MovableListener.h"
#include "../include/MovableFactory.h"
#include "../include/Scene.h"
#include "../include/SceneNode.h"
#include "../include/ObjectCamera.h"

namespace LostPeterEngine
{
    uint32 Movable::ms_nQueryFlags_Default = 0xFFFFFFFF;
	uint32 Movable::ms_nVisibilityFlags_Default	= 0xFFFFFFFF;

    Movable::Movable(const String& nameMovable, Scene* pScene)
        : Base(nameMovable)
        , m_pScene(pScene)
        , m_pMovableListener(nullptr)
        , m_pMovableFactory(nullptr)
		, m_pParentNode(nullptr)
        , m_nQueryFlags(ms_nQueryFlags_Default)
		, m_nVisibilityFlags(ms_nVisibilityFlags_Default)
		, m_bParentIsTagPoint(false)
		, m_bVisible(true)
		, m_bDebugDisplay(false)
        , m_bBeyondFarDistance(false)
        , m_bRenderingDisabled(false)
		, m_fUpperDistance(0.0f)
		, m_fSquaredUpperDistance(0.0f)
    {

    }

    Movable::~Movable()
    {
        if (m_pMovableListener)
		{
            m_pMovableListener->ObjectDestroyed(this);
        }

		if (m_pParentNode)
		{
			if (m_bParentIsTagPoint)
			{

			}
			else
			{
				static_cast<SceneNode*>(m_pParentNode)->DetachObject(this);
			}
		}
    }

    uint32 Movable::GetTypeFlags() const
	{
		if (m_pMovableFactory)
        {
            return m_pMovableFactory->GetTypeFlags();
        }
		return 0xFFFFFFFF;
	}

	SceneNode* Movable::GetParentSceneNode() const
	{	
		if (m_bParentIsTagPoint)
		{
			
		}
		return static_cast<SceneNode*>(m_pParentNode);
	}	

	bool Movable::IsVisible() const
	{
		if (!m_bVisible || m_bBeyondFarDistance || m_bRenderingDisabled)
			return false;



		return true;
	}	

	void Movable::NotifyAttached(Node* pParent, bool bIsTagPoint /*= false*/)
	{
		m_pParentNode = pParent;
		m_bParentIsTagPoint = bIsTagPoint;

		if (m_pMovableListener && pParent != m_pParentNode)
		{
			if (m_pParentNode)
				m_pMovableListener->ObjectAttached(this);
			else
				m_pMovableListener->ObjectDetached(this);
		}
	}

	void Movable::NotifyMoved()
	{
		if (m_pMovableListener)
		{
			m_pMovableListener->ObjectMoved(this);
		}
	}

	void Movable::DetachFromParent()
	{
		if (IsAttached())
		{
			if (m_bParentIsTagPoint)
			{

			}
			else
			{
				SceneNode* pSceneNode = static_cast<SceneNode*>(m_pParentNode);
				pSceneNode->DetachObject(this);
			}
		}
	}
	
	bool Movable::IsInScene() const
	{
		if (m_pParentNode)
		{
			if (m_bParentIsTagPoint)
			{
				
			}
			else
			{
				SceneNode* pSceneNode = static_cast<SceneNode*>(m_pParentNode);
				return pSceneNode->IsInSceneGraph();
			}
		}
		
		return false;
	}

	void Movable::NotifyCurrentCamera(ObjectCamera* pCamera)
	{
		if (m_pParentNode)
		{
			//if (pCamera->GetUseRenderingDistance() && m_fUpperDistance > 0)
			//{
				// float fRadius = GetBoundingRadius();
				// float fSquaredDepth = m_pParentNode->GetSquaredViewDepth(pCamera->GetLodCamera());
				// float maxDist = m_fUpperDistance + fRadius;
				// if (fSquaredDepth > FMath::Square(maxDist))
				// {
				// 	m_bBeyondFarDistance = true;
				// }
				// else
				// {
				// 	m_bBeyondFarDistance = false;
				// }
			//}
			//else
			//{
				m_bBeyondFarDistance = false;
			//}
		}
		m_bRenderingDisabled = m_pMovableListener && !m_pMovableListener->ObjectRendering(this, pCamera);
	}

	const FMatrix4&	Movable::GetNodeParentFullTransform() const
	{
		if (m_pParentNode)
		{
            return m_pParentNode->GetWorldTransformMatrix4();
        }

		return FMath::ms_mat4Unit;
	}

	const FAABB& Movable::GetBoundingAABBWorld(bool isDerive /*= false*/) const
	{
		if (isDerive)
		{
			m_aabbWorld = this->GetBoundingAABB();
			m_aabbWorld.TransformAffine(GetNodeParentFullTransform());
		}
		return m_aabbWorld;
	}

	const FSphere&	Movable::GetBoundingSphereWorld(bool isDerive /*= false*/) const
	{
		if (isDerive)
		{
			m_sphereWorld.SetRadius(GetBoundingRadius());
			m_sphereWorld.SetCenter(m_pParentNode->GetPositionWorld());
		}

		return m_sphereWorld;
	}
    
}; //LostPeterEngine