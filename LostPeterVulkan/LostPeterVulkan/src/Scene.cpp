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

#include "../include/Scene.h"
#include "../include/VulkanWindow.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneManager.h"
#include "../include/SceneNode.h"

namespace LostPeter
{
    Scene::Scene(uint _group, 
                 const String& nameScene)
        : Base(_group, nameScene)
    ////SceneManager/SceneNode
        , m_pSceneManager(nullptr)
        , m_pSceneNodeRoot(nullptr)
    ////Viewport
        , m_pViewportMain(nullptr)
    ////Object
        
    {

    }

    Scene::~Scene()
    {
        Destroy();
    }

    void Scene::Destroy()
    {
        UnloadScene();
    }

    bool Scene::LoadScene()
    {
        if (!IsGroupNameValid())
		{
            F_LogError("*********************** Scene::LoadScene: Group, Name is not valid, group: [%u, name: [%s] !", this->group, this->name.c_str());
            return false;
        }

        if (!SceneDataManager::GetSingleton().Parser(this->group, this->name, this))
        {
            F_LogError("*********************** Scene::LoadScene: Group, Parse scene file failed, group: [%u, name: [%s] !", this->group, this->name.c_str());
            return false;
        }

        return true;
    }

	void Scene::UnloadScene()
    {

    }


////SceneManager/SceneNode
    SceneNode*	Scene::GetSceneNodeRoot()
	{
		if (!m_pSceneNodeRoot)
		{
			m_pSceneNodeRoot = createSceneNodeImpl("SceneNodeRoot");
			m_pSceneNodeRoot->NotifyRootNode();
		}
		return m_pSceneNodeRoot;
	}

	SceneNode* Scene::GetSceneNode(const String& strName) const
	{
		SceneNodePtrMap::const_iterator itFind = m_mapSceneNodes.find(strName);
		if (itFind == m_mapSceneNodes.end())
		{
			F_LogError("Scene::GetSceneNode: SceneNode: [%s] not found !", strName.c_str());
			return nullptr;
		}
		return itFind->second;
	}

	bool Scene::HasSceneNode(const String& strName) const
	{
		return (m_mapSceneNodes.find(strName) != m_mapSceneNodes.end());
	}

	SceneNode* Scene::CreateSceneNode()
	{
		SceneNode* pSceneNode = createSceneNodeImpl();
		F_Assert(m_mapSceneNodes.find(pSceneNode->GetName()) == m_mapSceneNodes.end() && "Scene::CreateSceneNode")
		m_mapSceneNodes[pSceneNode->GetName()] = pSceneNode;
		return pSceneNode;
	}

	SceneNode* Scene::CreateSceneNode(const String& strName)
	{
		if (m_mapSceneNodes.find(strName) != m_mapSceneNodes.end())
		{
			F_LogError("Scene::CreateSceneNode: Scene node with the name: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		SceneNode* pSceneNode = createSceneNodeImpl(strName);
		m_mapSceneNodes[pSceneNode->GetName()] = pSceneNode;
		return pSceneNode;
	}

	void Scene::DestroySceneNode(const String& strName)
	{
		SceneNodePtrMap::iterator itFind = m_mapSceneNodes.find(strName);
		if (itFind == m_mapSceneNodes.end())
		{
			F_LogError("SceneManager::DestroySceneNode: SceneNode: [%s] not found !", strName.c_str());
			return;
		}

		for (SceneNodePtrSet::iterator it = m_setAutoTrackingSceneNodes.begin();
			 it != m_setAutoTrackingSceneNodes.end(); )
		{
			SceneNodePtrSet::iterator itCur = it++;
			SceneNode* pSceneNode = *itCur;

			if (pSceneNode->GetAutoTrackTarget() == itFind->second)
			{
				pSceneNode->SetAutoTracking(false);
			}
			else if (pSceneNode == itFind->second)
			{
				m_setAutoTrackingSceneNodes.erase(itCur);
			}
		}

		Node* pNodeParent = itFind->second->GetParent();
		if (pNodeParent)
		{
			pNodeParent->RemoveChild(itFind->second);
		}
		F_DELETE(itFind->second);
		m_mapSceneNodes.erase(itFind);
	}

	void Scene::DestroySceneNode(SceneNode* pSceneNode)
	{
		DestroySceneNode(pSceneNode->GetName());
	}

	void Scene::NotifyAutoTrackingSceneNode(SceneNode* pSceneNode, bool bAutoTrack)
	{
		if (bAutoTrack)
		{
			m_setAutoTrackingSceneNodes.insert(pSceneNode);
		}
		else
		{
			m_setAutoTrackingSceneNodes.erase(pSceneNode);
		}
	}

    SceneNode* Scene::createSceneNodeImpl()
	{
		return new SceneNode(this);
	}

	SceneNode* Scene::createSceneNodeImpl(const String& strName)
	{
		return new SceneNode(strName, this);
	}

////Viewport



////Object



}; //LostPeter