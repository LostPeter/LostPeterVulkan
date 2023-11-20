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

#include "../include/SceneNode.h"
#include "../include/Scene.h"
#include "../include/ObjectVisibleBoundsInfo.h"
#include "../include/ObjectCamera.h"
#include "../include/RenderQueue.h"

namespace LostPeterEngine
{
    SceneNode::SceneNode(Scene* pScene)
        : Node()
        , m_pScene(pScene)
		, m_bAABBSet(false)
		//, m_pWireBoundingBox(nullptr)
		, m_bShowBoundingBox(false)
		, m_bYawFixed(false)
		, m_pAutoTrackTarget(nullptr)
		, m_bIsInSceneGraph(false)
		, m_bVisible(true)
		, m_bVisibleIsInherit(true)
		, m_bDerivedVisible(true)
    {
        NeedUpdate();
    }

    SceneNode::SceneNode(const String& nameSceneNode, Scene* pScene)
        : Node(nameSceneNode)
        , m_pScene(pScene)
		, m_bAABBSet(false)
		//, m_pWireBoundingBox(nullptr)
		, m_bShowBoundingBox(false)
		, m_bYawFixed(false)
		, m_pAutoTrackTarget(nullptr)
		, m_bIsInSceneGraph(false)
		, m_bVisible(true)
		, m_bVisibleIsInherit(true)
		, m_bDerivedVisible(true)
    {
        NeedUpdate();
    }

    SceneNode::~SceneNode()
    {
		for (MovablePtrMap::iterator it = m_mapMovable.begin();
             it != m_mapMovable.end(); ++it)
		{
			Movable* pMovable = it->second;
			pMovable->NotifyAttached((SceneNode*)nullptr);
		}
		m_mapMovable.clear();
		//F_DELETE(m_pWireBoundingBox)
    }

    SceneNode* SceneNode::GetParentSceneNode() const
	{
		return static_cast<SceneNode*>(GetParent());
	}

	const FAABB& SceneNode::GetAABB()
	{
		if (m_bAABBSet)
		{
			return m_aabbSet;
		}
		else
		{
			m_aabbChild.SetNull();

			for(MovablePtrMap::iterator it = m_mapMovable.begin();
                it != m_mapMovable.end(); ++it)
			{
				Movable* pMovable = it->second;
				m_aabbChild.Merge(pMovable->GetBoundingAABB());
			}
			return m_aabbChild;
		}
	}

	void SceneNode::SetAABB(bool bSetAABB, const FAABB& aabb)
	{
		m_bAABBSet = bSetAABB;
		if (m_bAABBSet)
			m_aabbSet = aabb;
		else
			m_aabbSet.SetNull();
		NeedUpdate();
	}

	//void SceneNode::AddBoundingBoxToQueue(RenderQueue* pQueue)
	//{
		//if (!m_pWireBoundingBox)
		//	m_pWireBoundingBox = new WireBoundingBox(m_strName);
		//m_pWireBoundingBox->SetupBoundingBox(m_aabbWorld);
		//pQueue->AddRenderable(m_pWireBoundingBox);
	//}

	void SceneNode::SetInSceneGraph(bool bIsInGraph)
	{
		if (bIsInGraph != m_bIsInSceneGraph)
		{
			m_bIsInSceneGraph = bIsInGraph;
			for (NodePtrMap::iterator it = m_mapChild.begin();
				it != m_mapChild.end(); ++it)
			{
				SceneNode* pSceneNode = static_cast<SceneNode*>(it->second);
				pSceneNode->SetInSceneGraph(bIsInGraph);
			}
		}
	}

    bool SceneNode::IsVisible()
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_bDerivedVisible; 
	}	

	void SceneNode::SetVisible(bool bVisible)
	{
		if(m_bVisible != bVisible)
		{
			m_bVisible = bVisible;
			NeedUpdate();
		}		

		for (MovablePtrMap::iterator it = m_mapMovable.begin();
			 it != m_mapMovable.end();++it)
		{
			it->second->SetVisible(bVisible);
		}

		// if (bCascade)
		// {
		// 	for (NodePtrMap::iterator it = m_mapChild.begin();
		// 		it != m_mapChild.end(); ++it)
		// 	{
		// 		static_cast<SceneNode*>(it->second)->SetVisible(bVisible, bCascade);
		// 	}
		// }
	}

	void SceneNode::FlipVisibility(bool bCascade /*= true*/)
	{
		m_bVisible = !m_bVisible;
		NeedUpdate();

		for (MovablePtrMap::iterator it = m_mapMovable.begin();
			 it != m_mapMovable.end(); ++it)
		{
			it->second->SetVisible(!it->second->IsVisible());
		}

		if (bCascade)
		{
			for (NodePtrMap::iterator it = m_mapChild.begin();
				it != m_mapChild.end(); ++it)
			{
				static_cast<SceneNode*>(it->second)->FlipVisibility(bCascade);
			}
		}
	}

	void SceneNode::SetDebugDisplayEnabled(bool bEnabled,bool bCascade /*= true*/)
	{
		for (MovablePtrMap::iterator it = m_mapMovable.begin();
			it != m_mapMovable.end(); ++it)
		{
			it->second->SetDebugDisplayEnabled(bEnabled);
		}

		if (bCascade)
		{
			for (NodePtrMap::iterator it = m_mapChild.begin();
				it != m_mapChild.end(); ++it)
			{
				static_cast<SceneNode*>(it->second)->SetDebugDisplayEnabled(bEnabled, bCascade);
			}
		}
	}

	uint32 SceneNode::GetAttachedObjectCount() const
	{
		return static_cast<uint32>(m_mapMovable.size());
	}

	Movable* SceneNode::GetAttachedObject(uint32 nIndex)
	{
		if (nIndex < m_mapMovable.size())
		{
			MovablePtrMap::iterator it = m_mapMovable.begin();       
			while (nIndex--) 
				++it;
			return it->second;
		}
		return nullptr;
	}

	Movable* SceneNode::GetAttachedObject(const String& strName)
	{
		MovablePtrMap::iterator itFind = m_mapMovable.find(strName);
		if (itFind == m_mapMovable.end())
		{
			F_LogError("SceneNode::GetAttachedObject: Attached object: [%s] not found !", strName.c_str());
			return nullptr;
		}
		return itFind->second;
	}

	void SceneNode::AttachObject(Movable* pMovable)
	{
		if (pMovable->IsAttached())
		{
			F_LogError("SceneNode::AttachObject: Object: [%s] already attached to a SceneNode or a Bone !", pMovable->GetName().c_str());
			return;
		}
		pMovable->NotifyAttached(this);
		m_mapMovable.insert(MovablePtrMap::value_type(pMovable->GetName(), pMovable));
		NeedUpdate();
	}

	Movable* SceneNode::DetachObject(uint32 nIndex)
	{
		if (nIndex < m_mapMovable.size())
		{
			MovablePtrMap::iterator it = m_mapMovable.begin();          
			while (nIndex--)
				++it;
			Movable* pMovable = it->second;
			m_mapMovable.erase(it);
			pMovable->NotifyAttached((SceneNode*)nullptr);
			NeedUpdate();

			return pMovable;
		}
		return nullptr;
	}
	void SceneNode::DetachObject(Movable* pMovable)
	{
		DetachObject(pMovable->GetName());
	}
	Movable* SceneNode::DetachObject(const String& strName)
	{
		MovablePtrMap::iterator itFind = m_mapMovable.find(strName);
		if (itFind == m_mapMovable.end())
		{
			F_LogError("SceneNode::DetachObject: Object: [%s] is not attached to this node !", strName.c_str());
			return nullptr;
		}
		Movable* pMovable = itFind->second;
		m_mapMovable.erase(itFind);
		pMovable->NotifyAttached((SceneNode*)nullptr);
		NeedUpdate();

		return pMovable;
	}
	void SceneNode::DetachAllObjects()
	{
		for (MovablePtrMap::iterator it = m_mapMovable.begin();
			 it != m_mapMovable.end();++it)
		{
			Movable* pMovable = it->second;
			pMovable->NotifyAttached((SceneNode*)nullptr);
		}
		m_mapMovable.clear();
		NeedUpdate();
	}

	SceneNode* SceneNode::CreateChildSceneNode(const FVector3& vTrans /*= FMath::ms_v3Zero*/, 
											   const FQuaternion& qRot /*= FMath::ms_qUnit*/,
											   const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		return static_cast<SceneNode*>(this->CreateChild(vTrans, qRot, vScale));
	}
	SceneNode* SceneNode::CreateChildSceneNode(const String& strName, 
											   const FVector3& vTrans/* = FMath::ms_v3Zero*/, 
											   const FQuaternion& qRot /*= FMath::ms_qUnit*/,
											   const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		return static_cast<SceneNode*>(this->CreateChild(strName, vTrans, qRot, vScale));
	}

 	SceneNode* SceneNode::CreateChildSceneNodeByAngle(const FVector3& vTrans /*= FMath::ms_v3Zero*/, 
													  const FVector3& vAngle /*= FMath::ms_v3Zero*/,
												 	  const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		return static_cast<SceneNode*>(this->CreateChildByAngle(vTrans, vAngle, vScale));
	}
	SceneNode* SceneNode::CreateChildSceneNodeByAngle(const String& strName, 
													  const FVector3& vTrans /*= FMath::ms_v3Zero*/, 
													  const FVector3& vAngle /*= FMath::ms_v3Zero*/,
													  const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		return static_cast<SceneNode*>(this->CreateChildByAngle(strName, vTrans, vAngle, vScale));
	}

	void SceneNode::RemoveAndDestroyChild(const String& strName)
	{
		SceneNode* pChild = static_cast<SceneNode*>(GetChild(strName));
		pChild->RemoveAndDestroyAllChildren();
		RemoveChild(strName);
		pChild->GetScene()->DestroySceneNode(strName);
	}
	void SceneNode::RemoveAndDestroyChild(uint32 nIndex)
	{
		SceneNode* pChild = static_cast<SceneNode*>(GetChild(nIndex));
		pChild->RemoveAndDestroyAllChildren();

		RemoveChild(nIndex);
		pChild->GetScene()->DestroySceneNode(pChild->GetName());
	}
	void SceneNode::RemoveAndDestroyAllChildren()
	{
		for (NodePtrMap::iterator it = m_mapChild.begin();
			it != m_mapChild.end(); ++it)
		{
			SceneNode* pSceneNode = static_cast<SceneNode*>(it->second);
			pSceneNode->RemoveAndDestroyAllChildren();
			pSceneNode->GetScene()->DestroySceneNode(pSceneNode->GetName());
		}
		m_mapChild.clear();
		NeedUpdate();
	}

	void SceneNode::UpdateBounds()
	{
		m_aabbWorld.SetNull();
		if (m_bAABBSet)
		{
			m_aabbWorld = m_aabbSet;
			m_aabbWorld.TransformAffine(GetWorldTransformMatrix4());

			m_spereWorld.SetCenter(m_aabbWorld.GetCenter());
			FVector3 vBound = m_aabbWorld.GetMax() - m_aabbWorld.GetMin();
			m_spereWorld.SetRadius(FMath::Length(vBound) * 0.5f);
		}
		else
		{
			for(MovablePtrMap::iterator it = m_mapMovable.begin();
                it != m_mapMovable.end(); ++it)
			{
				Movable* pMovable = it->second;
				m_aabbWorld.Merge(pMovable->GetBoundingAABBWorld(true));
			}
			mergeChildrenAABB();
		}
	}

	void SceneNode::mergeChildrenAABB()
	{
		for (NodePtrMap::iterator it = m_mapChild.begin();
             it != m_mapChild.end(); ++it)
		{
			SceneNode* pSceneNode = static_cast<SceneNode*>(it->second);
			m_aabbWorld.Merge(pSceneNode->m_aabbWorld);
		}
		m_spereWorld.SetCenter(m_aabbWorld.GetCenter());
		FVector3 vBound = m_aabbWorld.GetMax() - m_aabbWorld.GetMin();
		m_spereWorld.SetRadius(FMath::Length(vBound) * 0.5f);
	}
	
	void SceneNode::FindVisibleObjects(ObjectCamera* pCamera, 
                                       RenderQueue* pRenderQueue,
									   ObjectVisibleBoundsInfo* pVisibleBounds,
									   bool bOnlyShadowCasters /*= false*/,
									   bool bIncludeChildren /*= true*/, 
									   bool bDisplayNodes /*= false*/)
	{
		// if (!pCamera->IsVisible(m_aabbWorld))
		// 	return;
		
		if (m_bDerivedVisible)
		{
			for (MovablePtrMap::iterator it = m_mapMovable.begin();
				 it != m_mapMovable.end(); ++it)
			{
				Movable* pMovable = it->second;

				pMovable->NotifyCurrentCamera(pCamera);
				if (pMovable->IsVisible() && 
					(!bOnlyShadowCasters /*|| pMovable->GetCastShadows()*/))
				{
					//pMovable->UpdateRenderQueue(pRenderQueue);
					if (pVisibleBounds)
					{
						pVisibleBounds->Merge(pMovable->GetBoundingAABBWorld(true), 
							                  pMovable->GetBoundingSphereWorld(true), 
                                              pCamera);
					}
				}
			}
		}

		if (bIncludeChildren)
		{
			NodePtrMap::iterator itChild, itChildend;
			itChildend = m_mapChild.end();
			for (itChild = m_mapChild.begin(); 
				itChild != itChildend; ++itChild)
			{
				SceneNode* pChild = static_cast<SceneNode*>(itChild->second);
				pChild->FindVisibleObjects(pCamera,
                                           pRenderQueue,
                                           pVisibleBounds,
							               bOnlyShadowCasters,
                                           bIncludeChildren,
                                           bDisplayNodes);
			}
		}

		if (bDisplayNodes)
		{
			//pRenderQueue->AddRenderable(this);
		}

		//if (m_bShowBoundingBox || (m_pScene && m_pScene->GetShowBoundingBoxes())) 
		//{ 
			//AddBoundingBoxToQueue(pRenderQueue);
		//}
	}

	void SceneNode::SetDirection(float x, float y, float z, FTransformType typeTransform /*= F_Transform_Local*/, const FVector3& vLocalDir /*= FMath::ms_v3UnitNegZ*/)
	{
		SetDirection(FVector3(x, y, z), typeTransform, vLocalDir);
	}

	void SceneNode::SetDirection(const FVector3& vDir, FTransformType typeTransform /*= F_Transform_Local*/, const FVector3& vLocalDir /*= FMath::ms_v3UnitNegZ*/)
	{
		if (vDir == FMath::ms_v3Zero)
			return;

		FVector3 vTargetDir = FMath::Normalize(vDir);
		switch (typeTransform)
		{
		case F_Transform_Parent:
			if (m_bRotIsInherit)
			{
				if (m_pParent)
				{
					vTargetDir = m_pParent->GetRotationWorld() * vTargetDir;
				}
			}
			break;
		case F_Transform_Local:
			vTargetDir = GetRotationWorld() * vTargetDir;
			break;
		case F_Transform_World:

			break;
		}

		FQuaternion qTargetRot;
		if (m_bYawFixed)
		{
			FVector3 vX = FMath::Normalize(FMath::Cross(m_vYawFixedAxis, vTargetDir));
			FVector3 vY = FMath::Normalize(FMath::Cross(vTargetDir, vX));
			FQuaternion qZToTarget = FMath::ToQuaternion(vX, vY, vTargetDir);

			if (vLocalDir == FMath::ms_v3UnitNegZ)
			{
				qTargetRot = FQuaternion(-qZToTarget.z, qZToTarget.w, qZToTarget.x, -qZToTarget.y);
			}
			else
			{
				FQuaternion vLocalToUnitZ = FMath::ToQuaternionFromSrc2Dst(vLocalDir, FMath::ms_v3UnitZ);
				qTargetRot = qZToTarget * vLocalToUnitZ;
			}
		}
		else
		{
			const FQuaternion& qCurrentOrient = GetRotationWorld();
			FVector3 vCurrentDir = qCurrentOrient * vLocalDir;
			if (FMath::Length2(vCurrentDir + vTargetDir) < 0.00005f)
			{
				qTargetRot = FQuaternion(qCurrentOrient.w, qCurrentOrient.x, -qCurrentOrient.y, -qCurrentOrient.z);
			}
			else
			{
				FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(vCurrentDir, vTargetDir);
				qTargetRot = qRot * qCurrentOrient;
			}
		}

		if (m_pParent && m_bRotIsInherit)
			SetRotationLocal(FMath::UnitInverseQuaternion(m_pParent->GetRotationWorld()) * qTargetRot);
		else
			SetRotationLocal(qTargetRot);
	}

	void SceneNode::LookAt(const FVector3& vTarget, FTransformType typeTransform, const FVector3& vLocalDir /*= FMath::ms_v3UnitNegZ*/)
	{
		FVector3 vOrigin;
		switch (typeTransform)
		{
		default:
		case F_Transform_World:
			vOrigin = GetPositionWorld();
			break;
		case F_Transform_Parent:
			vOrigin = m_vPosLocal;
			break;
		case F_Transform_Local:
			vOrigin = FMath::ms_v3Zero;
			break;
		}
		SetDirection(vTarget - vOrigin, typeTransform, vLocalDir);
	}

	void SceneNode::SetAutoTracking(bool bEnabled, SceneNode* pTarget /*= nullptr*/, const FVector3& vLocalDir /*= FMath::ms_v3UnitNegZ*/, const FVector3& vOffset /*= FMath::ms_v3Zero*/)
	{
		if (bEnabled)
		{
			m_pAutoTrackTarget = pTarget;
			m_vAutoTrackOffset = vOffset;
			m_vAutoTrackLocalDirection = vLocalDir;
		}
		else
		{
			m_pAutoTrackTarget = nullptr;
		}

		if (m_pScene)
		{
            m_pScene->NotifyAutoTrackingSceneNode(this, bEnabled);
        }
	}

	void SceneNode::AutoTrack()
	{
		if (m_pAutoTrackTarget)
		{
			LookAt(m_pAutoTrackTarget->GetPositionWorld() + m_vAutoTrackOffset, F_Transform_World, m_vAutoTrackLocalDirection);
			Update(true, true);
		}
	}

	void SceneNode::Yaw(float fAngle, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		if (m_bYawFixed)
		{
			Rotate(m_vYawFixedAxis, fAngle, typeTransform);
		}
		else
		{
			Rotate(FMath::ms_v3UnitY, fAngle, typeTransform);
		}
	}

	void SceneNode::Update(bool bUpdateChildren, bool bParentHasChanged)
	{
		Node::Update(bUpdateChildren, bParentHasChanged);
		UpdateBounds();
	}
	
	void SceneNode::setParent(Node* pParent)
	{
		Node::setParent(pParent);

		if (pParent)
		{
			SceneNode* sceneParent = static_cast<SceneNode*>(pParent);
			SetInSceneGraph(sceneParent->IsInSceneGraph());
		}
		else
		{
			SetInSceneGraph(false);
		}
	}

	void SceneNode::updateFromParentImpl() const
	{
		if (m_pParent && m_bVisibleIsInherit)
		{			
			bool bParentVisible = dynamic_cast<SceneNode*>(m_pParent)->IsVisible();
			m_bDerivedVisible = bParentVisible && m_bVisible;						
		}
		else
		{
			m_bDerivedVisible = m_bVisible;
		}

		Node::updateFromParentImpl();
		
		for (MovablePtrMap::const_iterator it = m_mapMovable.begin();
			it != m_mapMovable.end(); ++it)
		{
			Movable* pMovable = it->second;
			pMovable->NotifyMoved();
		}
	}

	Node* SceneNode::createChildImpl()
	{
		F_Assert(m_pScene && "SceneNode::createChildImpl")
		return m_pScene->CreateSceneNode();
	}
	Node* SceneNode::createChildImpl(const String& strName)
	{
		F_Assert(m_pScene && "SceneNode::createChildImpl")
		return m_pScene->CreateSceneNode(strName);
	}

}; //LostPeterEngine