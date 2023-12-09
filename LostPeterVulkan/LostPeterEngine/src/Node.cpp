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

#include "../include/Node.h"
#include "../include/NodeListener.h"

namespace LostPeterEngine
{
    uint32 Node::ms_nNextID = 0;
    String Node::GetNameAuto()
    {
        return "AutoName_" + FUtilString::SaveUInt(GetNextID());
    }

    NodePtrVector Node::ms_aNodeQueuedUpdates;

    Node::Node()
        : Node(GetNameAuto())
    {

    }
    Node::Node(const String& nameNode)
        : Base(nameNode)
        , m_pParent(nullptr)
        , m_pNodeListener(nullptr)
		, m_bNeedParentUpdate(false)
		, m_bNeedChildUpdate(false)		
		, m_bParentNotified(false)
		, m_bQueuedForUpdate(false)
		, m_vPosInitial(FMath::ms_v3Zero)
		, m_qRotInitial(FMath::ms_qUnit)
		, m_vScaleInitial(FMath::ms_v3One)
		, m_vPosLocal(FMath::ms_v3Zero)
		, m_qRotLocal(FMath::ms_qUnit)
		, m_vScaleLocal(FMath::ms_v3One)
		, m_bRotIsInherit(true)
		, m_bScaleIsInherit(true)
		, m_vPosWorld(FMath::ms_v3Zero)
		, m_qRotWorld(FMath::ms_qUnit)
		, m_vScaleWorld(FMath::ms_v3One)
		, m_bTransformWorldOutOfDate(true)
    {

    }   
    Node::~Node()
    {
        if(m_pNodeListener)
		{
			m_pNodeListener->NodeDestroyed(this);
		}
		RemoveAllChildren();
		if (m_pParent)
		{
            m_pParent->RemoveChild(this);
        }

		if (m_bQueuedForUpdate)
		{
			NodePtrVector::iterator it = std::find(ms_aNodeQueuedUpdates.begin(), ms_aNodeQueuedUpdates.end(), this);
			if (it != ms_aNodeQueuedUpdates.end())
			{
				*it = ms_aNodeQueuedUpdates.back();
				ms_aNodeQueuedUpdates.pop_back();
			}
		}
    }

    const FVector3&	Node::GetPositionWorld() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_vPosWorld;
	}
	void Node::SetPositionWorld(const FVector3& vPosWorld)
	{
		SetPositionLocal(m_pParent->ConvertPositionFromWorldToLocal(vPosWorld));
	}	

	const FQuaternion& Node::GetRotationWorld() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_qRotWorld;
	}

	void Node::SetRotationWorld(const FQuaternion& qRotWorld)
	{
		SetRotationLocal(m_pParent->ConvertRotationFromWorldToLocal(qRotWorld));
	}

	const FVector3&	Node::GetScaleWorld() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_vScaleWorld;
	}

	void Node::Translate(const FVector3& vMove, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		switch (typeTransform)
		{
		case F_Transform_Local:
			m_vPosLocal += m_qRotLocal * vMove;
			break;
		case F_Transform_World:
			if (m_pParent)
			{
				m_vPosLocal += (FMath::InverseQuaternion(m_pParent->GetRotationWorld()) * vMove) / m_pParent->GetScaleWorld();
			}
			else
			{
				m_vPosLocal += vMove;
			}
			break;
		case F_Transform_Parent:
			m_vPosLocal += vMove;
			break;
		}
		NeedUpdate();
	}
	void Node::Translate(float x, float y, float z, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		Translate(FVector3(x, y, z), typeTransform);
	}
	void Node::Translate(const FMatrix3& mat3, const FVector3& vMove, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		FVector3 vM = mat3 * vMove;
		Translate(vM, typeTransform);
	}
	void Node::Translate(const FMatrix3& mat3, float x, float y, float z, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		Translate(mat3, FVector3(x, y, z), typeTransform);
	}

	void Node::Rotate(const FVector3& vAxis, float fAngle, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		FQuaternion qRot = FMath::ToQuaternionFromAngleAxis(fAngle, vAxis);
		Rotate(qRot, typeTransform);
	}
	void Node::Rotate(const FQuaternion& qRot, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		FQuaternion qRotNorm = FMath::Normalize(qRot);
		switch(typeTransform)
		{
		case F_Transform_Parent:
			m_qRotLocal = qRotNorm * m_qRotLocal;
			break;
		case F_Transform_World:
			m_qRotLocal = m_qRotLocal * FMath::InverseQuaternion(GetRotationWorld()) * qRotNorm * GetRotationWorld();
			break;
		case F_Transform_Local:
			m_qRotLocal = m_qRotLocal * qRotNorm;
			break;
		}
		NeedUpdate();
	}
	void Node::Roll(float fAngle, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		Rotate(FMath::ms_v3UnitZ, fAngle, typeTransform);
	}
	void Node::Pitch(float fAngle, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		Rotate(FMath::ms_v3UnitX, fAngle, typeTransform);
	}
	void Node::Yaw(float fAngle, FTransformType typeTransform /*= F_Transform_Local*/)
	{
		Rotate(FMath::ms_v3UnitY, fAngle, typeTransform);
	}

	const FMatrix4& Node::GetWorldTransformMatrix4() const
	{
		if (m_bTransformWorldOutOfDate)
		{
            m_mat4TransformWorld = FMath::ToMatrix4(GetPositionWorld(), GetScaleWorld(), GetRotationWorld());
			m_bTransformWorldOutOfDate = false;
		}
		return m_mat4TransformWorld;
	}

	FVector3 Node::ConvertPositionFromWorldToLocal(const FVector3& vPosWorld)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return FMath::InverseQuaternion(m_qRotWorld) * (vPosWorld - m_vPosWorld) / m_vScaleWorld;
	}
	FVector3 Node::ConvertPositionFromLocalToWorld(const FVector3& vPosLocal)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return (m_qRotWorld * vPosLocal * m_vScaleWorld) + m_vPosWorld;
	}

	FQuaternion	Node::ConvertRotationFromWorldToLocal(const FQuaternion& qRotWorld)
	{		
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return FMath::InverseQuaternion(m_qRotWorld) * qRotWorld;
	}
	FQuaternion	Node::ConvertRotationFromLocalToWorld(const FQuaternion& qRotLocal)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_qRotWorld * qRotLocal;
	}
	
	Node* Node::CreateChild(const FVector3& vTranslate /*= FMath::ms_v3Zero*/, 
						    const FQuaternion& qRotate /*= FMath::ms_qUnit*/, 
							const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		Node* pNodeNew = createChildImpl();
		pNodeNew->SetPositionLocalOnly(vTranslate);
		pNodeNew->SetRotationLocalOnly(qRotate);
		pNodeNew->SetScaleLocalOnly(vScale);
		pNodeNew->NeedUpdate();
		this->AddChild(pNodeNew);

		return pNodeNew;
	}

	Node* Node::CreateChild(const String& strName, 
							const FVector3& vTranslate /*= FMath::ms_v3Zero*/, 
							const FQuaternion& qRotate /*= FMath::ms_qUnit*/, 
							const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		Node* pNodeNew = createChildImpl(strName);
		pNodeNew->SetPositionLocalOnly(vTranslate);
		pNodeNew->SetRotationLocalOnly(qRotate);
		pNodeNew->SetScaleLocalOnly(vScale);
		pNodeNew->NeedUpdate();
		this->AddChild(pNodeNew);
		
		return pNodeNew;
	}

	Node* Node::CreateChildByAngle(const FVector3& vTranslate /*= FMath::ms_v3Zero*/, 
								   const FVector3& vAngle /*= FMath::ms_v3Zero*/, 
								   const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		Node* pNodeNew = createChildImpl();
		pNodeNew->SetPositionLocalOnly(vTranslate);
		pNodeNew->SetAngleLocalOnly(vAngle);
		pNodeNew->SetScaleLocalOnly(vScale);
		pNodeNew->NeedUpdate();
		this->AddChild(pNodeNew);

		return pNodeNew;
	}
	Node* Node::CreateChildByAngle(const String& strName, 
								   const FVector3& vTranslate /*= FMath::ms_v3Zero*/,
								   const FVector3& vAngle /*= FMath::ms_v3Zero*/, 
								   const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		Node* pNodeNew = createChildImpl(strName);
		pNodeNew->SetPositionLocalOnly(vTranslate);
		pNodeNew->SetAngleLocalOnly(vAngle);
		pNodeNew->SetScaleLocalOnly(vScale);
		pNodeNew->NeedUpdate();
		this->AddChild(pNodeNew);
		
		return pNodeNew;
	}

	uint32 Node::GetChildrenCount() const
	{
		return static_cast<uint32>(m_mapChild.size());
	}
	Node* Node::GetChild(uint32 nIndex) const
	{
		if (nIndex < m_mapChild.size())
		{
			NodePtrMap::const_iterator it = m_mapChild.begin();
			while (nIndex--) 
                ++it;
			return it->second;
		}
		return nullptr;
	}
	Node* Node::GetChild(const String& strName) const
	{
		NodePtrMap::const_iterator itFind = m_mapChild.find(strName);
		if (itFind == m_mapChild.end())
		{
			F_LogError("*********************** Node::GetChild: Child node named: [%s] does not exist !", strName.c_str());
			return nullptr;
		}

		return itFind->second;
	}
	void Node::AddChild(Node* pChild)
	{
		if (pChild->m_pParent)
		{
			F_LogError("*********************** Node::AddChild: Node: [%s] already was a child of: [%s] !", pChild->GetName().c_str(), pChild->m_pParent->GetName().c_str());
			return;
		}

		m_mapChild.insert(NodePtrMap::value_type(pChild->GetName(), pChild));
		pChild->setParent(this);
	}
	Node* Node::RemoveChild(uint32 nIndex)
	{
		Node* pNodeRet;
		if (nIndex < m_mapChild.size())
		{
			NodePtrMap::iterator it = m_mapChild.begin();
			while (nIndex--) 
                ++it;
			pNodeRet = it->second;
			CancelUpdate(pNodeRet);

			m_mapChild.erase(it);
			pNodeRet->setParent(nullptr);
			return pNodeRet;
		}

		F_LogError("*********************** Node::RemoveChild: Child index out of bounds !");
		return nullptr;
	}	
	Node* Node::RemoveChild(Node* pChild)
	{
		if (pChild)
		{
			NodePtrMap::iterator itFind = m_mapChild.find(pChild->GetName());
			if (itFind != m_mapChild.end() && itFind->second == pChild)
			{
				CancelUpdate(pChild);

				m_mapChild.erase(itFind);
				pChild->setParent(nullptr);
			}
		}
		return pChild;
	}
	Node* Node::RemoveChild(const String& strName)
	{
		NodePtrMap::iterator itFind = m_mapChild.find(strName);
		if (itFind == m_mapChild.end())
		{
			F_LogError("*********************** Node::RemoveChild: Child node named: [%s] does not exist !", strName.c_str());
			return nullptr;
		}
		Node* pNodeRet = itFind->second;
		CancelUpdate(pNodeRet);
		m_mapChild.erase(itFind);
		pNodeRet->setParent(nullptr);

		return pNodeRet;
	}
	void Node::RemoveAllChildren()
	{
		for (NodePtrMap::iterator it = m_mapChild.begin();
			it != m_mapChild.end(); ++it)
		{
			it->second->setParent(nullptr);
		}
		m_mapChild.clear();
		m_setNeedToUpdate.clear();
	}	
	
	void Node::Update(bool bUpdateChildren, bool bParentHasChanged)
	{
		m_bParentNotified = false;

		if (!bUpdateChildren && 
            !m_bNeedParentUpdate && 
            !m_bNeedChildUpdate && 
            !bParentHasChanged)
		{
			return;
		}

		if (m_bNeedParentUpdate || bParentHasChanged)
		{
			updateFromParent();
		}

		if (m_bNeedChildUpdate || bParentHasChanged)
		{
			for (NodePtrMap::iterator it = m_mapChild.begin();
				 it != m_mapChild.end(); ++it)
			{
				Node* pNodeChild = it->second;
				pNodeChild->Update(true, true);
			}
			m_setNeedToUpdate.clear();
		}
		else
		{
			for (NodePtrSet::iterator it = m_setNeedToUpdate.begin();
				 it != m_setNeedToUpdate.end(); ++it)
			{
				Node* pNodeChild = *it;
				pNodeChild->Update(true, false);
			}
			m_setNeedToUpdate.clear();
		}

		m_bNeedChildUpdate = false;
	}

	void Node::NeedUpdate(bool bForceParentUpdate /*= false*/)
	{
		m_bNeedParentUpdate = true;
		m_bNeedChildUpdate = true;
		m_bTransformWorldOutOfDate = true;

		if (m_pParent && (!m_bParentNotified || bForceParentUpdate))
		{
			m_pParent->RequestUpdate(this, bForceParentUpdate);
			m_bParentNotified = true ;
		}
		m_setNeedToUpdate.clear();
	}

	void Node::RequestUpdate(Node* pChild, bool bForceParentUpdate /*= false*/)
	{
		if (m_bNeedChildUpdate)
		{
			return;
		}

		m_setNeedToUpdate.insert(pChild);
		if (m_pParent && (!m_bParentNotified || bForceParentUpdate))
		{
			m_pParent->RequestUpdate(this, bForceParentUpdate);
			m_bParentNotified = true ;
		}
	}

	void Node::CancelUpdate(Node* pChild)
	{
		m_setNeedToUpdate.erase(pChild);
		if (m_setNeedToUpdate.empty() && m_pParent && !m_bNeedChildUpdate)
		{
			m_pParent->CancelUpdate(this);
			m_bParentNotified = false ;
		}
	}

	void Node::QueueNeedUpdate(Node* pNode)
	{
		if (!pNode->m_bQueuedForUpdate)
		{
			pNode->m_bQueuedForUpdate = true;
			ms_aNodeQueuedUpdates.push_back(pNode);
		}
	}

	void Node::ProcessQueuedUpdates()
	{
		for (NodePtrVector::iterator it = ms_aNodeQueuedUpdates.begin();
			 it != ms_aNodeQueuedUpdates.end(); ++it)
		{
			Node* pNode = *it;
			pNode->m_bQueuedForUpdate = false;
			pNode->NeedUpdate(true);
		}
		ms_aNodeQueuedUpdates.clear();
	}

	void Node::setParent(Node* pParent)
	{
		m_pParent = pParent;
		m_bParentNotified = false;
		NeedUpdate();

		if (m_pNodeListener && pParent != m_pParent)
		{
			if (m_pParent)
				m_pNodeListener->NodeAttached(this);
			else
				m_pNodeListener->NodeDetached(this);
		}
	}

	void Node::updateFromParent() const
	{
		updateFromParentImpl();

		if (m_pNodeListener)
		{
			m_pNodeListener->NodeUpdated(this);
		}
	}

	void Node::updateFromParentImpl() const
	{
		if (m_pParent)
		{
			const FQuaternion& rotWorldParent= m_pParent->GetRotationWorld();
			if (m_bRotIsInherit)
			{
				m_qRotWorld = rotWorldParent * m_qRotLocal;
			}
			else
			{
				m_qRotWorld = m_qRotLocal;
			}

			const FVector3& vScaleParent = m_pParent->GetScaleWorld();
			if (m_bScaleIsInherit)
			{
				m_vScaleWorld = vScaleParent * m_vScaleLocal;
			}
			else
			{
				m_vScaleWorld = m_vScaleLocal;
			}

			m_vPosWorld = rotWorldParent * (vScaleParent * m_vPosLocal);
			m_vPosWorld += m_pParent->GetPositionWorld();
		}
		else
		{
            m_vPosWorld = m_vPosLocal;
			m_qRotWorld = m_qRotLocal;
			m_vScaleWorld	= m_vScaleLocal;
		}

		m_bTransformWorldOutOfDate = true;
		m_bNeedParentUpdate	= false;
	}	

}; //LostPeterEngine