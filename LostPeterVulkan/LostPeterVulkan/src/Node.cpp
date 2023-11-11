/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Node.h"
#include "../include/VulkanWindow.h"
#include "../include/NodeListener.h"

namespace LostPeter
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
		, m_vPos(FMath::ms_v3Zero)
		, m_vScale(FMath::ms_v3One)
		, m_qRot(FMath::ms_qUnit)
		, m_bInheritRot(true)
		, m_bInheritScale(true)
		, m_vDerivedPos(FMath::ms_v3Zero)
		, m_vDerivedScale(FMath::ms_v3One)
		, m_qDerivedRot(FMath::ms_qUnit)
		, m_vInitialPos(FMath::ms_v3Zero)
		, m_vInitialScale(FMath::ms_v3One)
		, m_qInitialRot(FMath::ms_qUnit)
		, m_bCachedTransformOutOfDate(true)
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

    const FVector3&	Node::GetDerivedPosition() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_vDerivedPos;
	}

	void Node::SetDerivedPosition(const FVector3& vPos)
	{
		SetPosition(m_pParent->ConvertWorldToLocalPosition(vPos));
	}	

	const FQuaternion& Node::GetDerivedOrientation() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_qDerivedRot;
	}

	void Node::SetDerivedOrientation(const FQuaternion& qRot)
	{
		SetOrientation(m_pParent->ConvertWorldToLocalOrientation(qRot));
	}

	const FVector3&	Node::GetDerivedScale() const
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_vDerivedScale;
	}

	void Node::Translate(const FVector3& vDis, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		switch (typeTransform)
		{
		case F_Transform_Local:
			m_vPos += m_qRot * vDis;
			break;
		case F_Transform_World:
			if (m_pParent)
			{
				m_vPos += (FMath::InverseQuaternion(m_pParent->GetDerivedOrientation()) * vDis) / m_pParent->GetDerivedScale();
			}
			else
			{
				m_vPos += vDis;
			}
			break;
		case F_Transform_Parent:
			m_vPos += vDis;
			break;
		}

		NeedUpdate();
	}

	void Node::Translate(float x, float y, float z, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		Translate(FVector3(x, y, z), typeTransform);
	}

	void Node::Translate(const FMatrix3& mat3Axes, const FVector3& vMove, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		FVector3 derived = mat3Axes * vMove;
		Translate(derived, typeTransform);
	}

	void Node::Translate(const FMatrix3& mat3Axes, float x, float y, float z, FTransformType typeTransform /*= F_Transform_Parent*/)
	{
		Translate(mat3Axes, FVector3(x, y, z), typeTransform);
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
			m_qRot = qRotNorm * m_qRot;
			break;
		case F_Transform_World:
			m_qRot = m_qRot * FMath::InverseQuaternion(GetDerivedOrientation()) * qRotNorm * GetDerivedOrientation();
			break;
		case F_Transform_Local:
			m_qRot = m_qRot * qRotNorm;
			break;
		}
		NeedUpdate();
	}

	const FMatrix4& Node::GetFullTransform() const
	{
		if (m_bCachedTransformOutOfDate)
		{
            m_matCachedTransform = FMath::ToMatrix4(GetDerivedPosition(), GetDerivedScale(), GetDerivedOrientation());
			m_bCachedTransformOutOfDate = false;
		}
		return m_matCachedTransform;
	}

	FVector3 Node::ConvertWorldToLocalPosition(const FVector3& vPosWorld)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return FMath::InverseQuaternion(m_qDerivedRot) * (vPosWorld - m_vDerivedPos) / m_vDerivedScale;
	}

	FVector3 Node::ConvertLocalToWorldPosition(const FVector3& vPosLocal)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return (m_qDerivedRot * vPosLocal * m_vDerivedScale) + m_vDerivedPos;
	}

	FQuaternion	Node::ConvertWorldToLocalOrientation(const FQuaternion& qRotWorld)
	{		
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return FMath::InverseQuaternion(m_qDerivedRot) * qRotWorld;
	}

	FQuaternion	Node::ConvertLocalToWorldOrientation(const FQuaternion& qRotLocal)
	{
		if (m_bNeedParentUpdate)
		{
			updateFromParent();
		}
		return m_qDerivedRot * qRotLocal;
	}
	
	Node* Node::CreateChild(const FVector3& vTranslate /*= FMath::ms_v3Zero*/, const FQuaternion& qRotate /*= FMath::ms_qUnit*/)
	{
		Node* pNodeNew = createChildImpl();
		pNodeNew->Translate(vTranslate);
		pNodeNew->Rotate(qRotate);
		this->AddChild(pNodeNew);

		return pNodeNew;
	}

	Node* Node::CreateChild(const String& strName, const FVector3& vTranslate /*= FMath::ms_v3Zero*/, const FQuaternion& qRotate /*= FMath::ms_qUnit*/)
	{
		Node* pNodeNew = createChildImpl(strName);
		pNodeNew->Translate(vTranslate);
		pNodeNew->Rotate(qRotate);
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
			F_LogError("Node::GetChild: Child node named: [%s] does not exist !", strName.c_str());
			return nullptr;
		}

		return itFind->second;
	}

	void Node::AddChild(Node* pChild)
	{
		if (pChild->m_pParent)
		{
			F_LogError("Node::AddChild: Node: [%s] already was a child of: [%s] !", pChild->GetName().c_str(), pChild->m_pParent->GetName().c_str());
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

		F_LogError("Node::RemoveChild: Child index out of bounds !");
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
			F_LogError("Node::RemoveChild: Child node named: [%s] does not exist !", strName.c_str());
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
		m_bCachedTransformOutOfDate = true;

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
			const FQuaternion& parentOrientation = m_pParent->GetDerivedOrientation();
			if (m_bInheritRot)
			{
				m_qDerivedRot = parentOrientation * m_qRot;
			}
			else
			{
				m_qDerivedRot = m_qRot;
			}

			const FVector3& vScaleParent = m_pParent->GetDerivedScale();
			if (m_bInheritScale)
			{
				m_vDerivedScale = vScaleParent * m_vScale;
			}
			else
			{
				m_vDerivedScale = m_vScale;
			}

			m_vDerivedPos = parentOrientation * (vScaleParent * m_vPos);
			m_vDerivedPos += m_pParent->GetDerivedPosition();
		}
		else
		{
            m_vDerivedPos = m_vPos;
			m_qDerivedRot = m_qRot;
			m_vDerivedScale	= m_vScale;
		}

		m_bCachedTransformOutOfDate = true;
		m_bNeedParentUpdate	= false;
	}	

}; //LostPeter