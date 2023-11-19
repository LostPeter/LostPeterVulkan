/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneDataManager.h"
#include "../include/SceneSerializer.h"
#include "../include/SceneDataSerializer.h"
#include "../include/SceneManager.h"
#include "../include/Scene.h"

template<> LostPeter::SceneDataManager* LostPeterFoundation::FSingleton<LostPeter::SceneDataManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    SceneDataManager* SceneDataManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SceneDataManager& SceneDataManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SceneDataManager::GetSingleton")
		return (*ms_Singleton);     
	}

    SceneDataManager::SceneDataManager()
        : Base("SceneDataManager")
        , m_pSceneSerializer(nullptr)
    {
        m_pSceneDataSerializer = new SceneDataSerializer(this);
    }
    SceneDataManager::~SceneDataManager()
    {
        Destroy();
    }

    void SceneDataManager::Destroy()
    {
        F_DELETE(m_pSceneSerializer)
        F_DELETE(m_pSceneDataSerializer)

    }

    bool SceneDataManager::Init(uint nGroup, const String& strNameCfg)
	{	
		//1> Scene Cfg Path 
        String strPathCfgScene = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgScene.empty())
        {
            F_LogError("*********************** SceneDataManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Scene Serializer
        m_pSceneSerializer = new SceneSerializer();
        if (!m_pSceneSerializer->LoadFile(strPathCfgScene))
        {
            F_LogError("*********************** SceneDataManager::Init: Load file scene cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

		return true;
	}

    bool SceneDataManager::LoadSceneAll()
    {
        if (m_pSceneSerializer == nullptr)
            return false;

        SceneInfoPtrVector& aSceneInfos = m_pSceneSerializer->GetSceneInfoPtrVector();
        for (SceneInfoPtrVector::iterator it = aSceneInfos.begin();
             it != aSceneInfos.end(); ++it)
        {
            if (!loadScene(*it))
                continue;
        }

        return true;
    }
    Scene* SceneDataManager::LoadScene(uint nGroup, const String& strName)
    {
        if (m_pSceneSerializer == nullptr)
            return nullptr;

        Scene* pScene = GetScene(nGroup, strName);
        if (pScene == nullptr)
        {
            SceneInfo* pSceneInfo = m_pSceneSerializer->GetSceneInfo(nGroup, strName);
            if (pSceneInfo == nullptr)
            {
                F_LogError("*********************** SceneDataManager::LoadScene: Can not find scene info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadScene(pSceneInfo))
            {
                return nullptr;
            }
        }
        pScene->AddRef();
        return pScene;
    }
    Scene* SceneDataManager::loadScene(SceneInfo* pSI)
    {
        Scene* pScene = new Scene(pSI->group,
                                  pSI->nameScene);
        if (!pScene->LoadScene())
        {
            F_LogError("*********************** SceneDataManager::loadScene: Load scene failed, name: [%s], path: [%s] !", pSI->nameScene.c_str(), pSI->pathScene.c_str());
            F_DELETE(pScene)
            return nullptr;
        }

        if (AddScene(pSI->group, pScene))
        {
            F_LogInfo("SceneDataManager::loadScene: Load scene success, [%u]-[%s]-[%s] !", 
                      pSI->group, 
                      pSI->nameScene.c_str(), 
                      pSI->pathScene.c_str());
        }
        return pScene;
    }

    void SceneDataManager::UnloadScene(Scene* pScene)
    {
        if (pScene == nullptr)
            return;
        pScene->DelRef();
        if (!HasRef())
        {
            DeleteScene(pScene->GetGroup(), pScene->GetName());
        }
    }

    bool SceneDataManager::HasScene(uint nGroup, const String& strName)
    {
        return GetScene(nGroup, strName) != nullptr;
    }

    Scene* SceneDataManager::GetScene(uint nGroup, const String& strName)
    {
        SceneGroupPtrMap::iterator itFindGroup = m_mapSceneGroup.find(nGroup);
        if (itFindGroup == m_mapSceneGroup.end())
        {
            return nullptr;
        }

        ScenePtrMap::iterator itFindScene = itFindGroup->second.find(strName);
        if (itFindScene == itFindGroup->second.end())
        {
            return nullptr;
        }
        return itFindScene->second;
    }

    bool SceneDataManager::AddScene(uint nGroup, Scene* pScene)
    {
        SceneGroupPtrMap::iterator itFind = m_mapSceneGroup.find(nGroup);
        if (itFind == m_mapSceneGroup.end())
        {
            ScenePtrMap mapScene;
            m_mapSceneGroup[nGroup] = mapScene;
            itFind = m_mapSceneGroup.find(nGroup);
        }
        const String& strName = pScene->GetName();
        ScenePtrMap::iterator itFindScene = itFind->second.find(strName);
        if (itFindScene != itFind->second.end())
        {
            F_LogError("*********************** SceneDataManager::AddScene: Scene name already exist: [%s] !", strName.c_str());
            F_DELETE(pScene)
            return false;
        }

        itFind->second.insert(ScenePtrMap::value_type(strName, pScene));
        m_aScene.push_back(pScene);
        return true;
    }

    void SceneDataManager::DeleteScene(uint nGroup, const String& strName)
    {
        SceneGroupPtrMap::iterator itFind = m_mapSceneGroup.find(nGroup);
        if (itFind == m_mapSceneGroup.end())
        {
            return;
        }

        ScenePtrMap::iterator itFindScene = itFind->second.find(strName);
        if (itFindScene != itFind->second.end())
        {
            ScenePtrVector::iterator itFindA = std::find(m_aScene.begin(), m_aScene.end(), itFindScene->second);
            if (itFindA != m_aScene.end())
                m_aScene.erase(itFindA);
            F_DELETE(itFindScene->second)
            itFind->second.erase(itFindScene);
        }
    }

    void SceneDataManager::DeleteSceneAll()
    {
        for (SceneGroupPtrMap::iterator it = m_mapSceneGroup.begin();
             it != m_mapSceneGroup.end(); ++it)
        {
            ScenePtrMap& mapScene = it->second;
            for (ScenePtrMap::iterator itScene = mapScene.begin(); 
                 itScene != mapScene.end(); ++itScene)
            {
                F_DELETE(itScene->second)
            }
        }
        m_aScene.clear();
        m_mapSceneGroup.clear();
    }


    bool SceneDataManager::Parser(uint32 nGroup, const String& strName, Scene* pScene)
    {
        return m_pSceneDataSerializer->Parser(nGroup, strName, pScene, nullptr);
    }
    bool SceneDataManager::Parser(uint32 nGroup, const String& strName, ScenePtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->Parser(nGroup, strName, nullptr, pRet);
    }

    bool SceneDataManager::ParserXML(uint32 nGroup, const String& strName, ScenePtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserXML(nGroup, strName, pRet);
    }
    bool SceneDataManager::ParserXML(const char* szFilePath, ScenePtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserXML(szFilePath, nullptr, pRet);
    }

    bool SceneDataManager::ParserBinary(uint32 nGroup, const String& strName, ScenePtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserBinary(nGroup, strName, pRet);
    }
    bool SceneDataManager::ParserBinary(const char* szFilePath, ScenePtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserBinary(szFilePath, nullptr, pRet);
    }

    bool SceneDataManager::SaveXML(Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveXML(pScene);
    }
    bool SceneDataManager::SaveXML(uint32 nGroup, Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveXML(nGroup, pScene);
    }
    bool SceneDataManager::SaveXML(const char* szFilePath, ScenePtrVector& aSA)
    {
        return m_pSceneDataSerializer->SaveXML(szFilePath, aSA);
    }

    bool SceneDataManager::SaveBinary(Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveBinary(pScene);
    }
    bool SceneDataManager::SaveBinary(uint32 nGroup, Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveBinary(nGroup, pScene);
    }
    bool SceneDataManager::SaveBinary(const char* szFilePath, ScenePtrVector& aSA)
    {
        return m_pSceneDataSerializer->SaveBinary(szFilePath, aSA);
    }


}; //LostPeter