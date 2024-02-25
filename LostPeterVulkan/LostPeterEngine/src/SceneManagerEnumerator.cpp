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

#include "../include/SceneManagerEnumerator.h"
#include "../include/SceneSerializer.h"
#include "../include/Scene.h"
#include "../include/SceneManager.h"

template<> LostPeterEngine::SceneManagerEnumerator* LostPeterFoundation::FSingleton<LostPeterEngine::SceneManagerEnumerator>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
	SceneManagerEnumerator* SceneManagerEnumerator::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SceneManagerEnumerator& SceneManagerEnumerator::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SceneManagerEnumerator::GetSingleton")
		return (*ms_Singleton);     
	}

	const String SceneManagerEnumerator::ms_strSceneConfigName = "Cfg_Scene.xml";

	static const int s_nCountDefaults = 1;
    static uint32 s_nGroupDefault = FPathManager::PathGroup_Scene;
    static String s_aNameDefaults[s_nCountDefaults] = 
    {
        "Default",
    };
    const String& SceneManagerEnumerator::GetSceneName_Default()
    {
        return s_aNameDefaults[0];
    }

    SceneManagerEnumerator::SceneManagerEnumerator()
        : Base("SceneManagerEnumerator")
		, m_pSceneSerializer(nullptr)
    {
		m_pSceneSerializer = new SceneSerializer();
    }
    SceneManagerEnumerator::~SceneManagerEnumerator()
    {
		F_DELETE(m_pSceneSerializer)
		Destroy();
    }

	bool SceneManagerEnumerator::IsSceneDefault(Scene* pScene)
    {
        for (ScenePtrMap::iterator it = m_mapSceneDefaults.begin(); 
             it != m_mapSceneDefaults.end(); ++it)
        {
            if (it->second == pScene)
                return true;
        }
        return false;
    }
    Scene* SceneManagerEnumerator::GetSceneDefault(const String& strName)
    {
        ScenePtrMap::iterator itFind = m_mapSceneDefaults.find(strName);
        if (itFind == m_mapSceneDefaults.end())
            return nullptr;
        return itFind->second;
    }
    Scene* SceneManagerEnumerator::GetSceneDefault(uint32 nGroup, const String& strName)
    {
        if (nGroup != s_nGroupDefault)
            return nullptr;
        return GetSceneDefault(strName);
    }
    Scene* SceneManagerEnumerator::GetScene_Default()
    {
        return GetSceneDefault(s_aNameDefaults[0]);
    }

	void SceneManagerEnumerator::Destroy()
	{
        DeleteSceneAll();
        
        destroySceneDefaults();
	}

    bool SceneManagerEnumerator::Init(uint32 nGroup, const String& strNameCfg)
	{	
		Destroy();

        //1> Scene Cfg Path 
        String strPathCfgScene = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgScene.empty())
        {
            F_LogError("*********************** SceneManagerEnumerator::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Scene Serializer
        if (!m_pSceneSerializer->LoadFile(strPathCfgScene))
        {
            F_LogError("*********************** SceneManagerEnumerator::Init: Load file scene cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }
        
        //3> Scene Default
        if (!initSceneDefaults())
        {
            F_LogError("*********************** SceneManagerEnumerator::Init: initSceneDefaults failed !");
            return false;
        }

        return true;
	}
	bool SceneManagerEnumerator::initSceneDefaults()
    {
        int count = s_nCountDefaults;
        for (int i = 0; i < count; i++)
        {
            String& nameScene = s_aNameDefaults[i];
            Scene* pScene = loadScene(s_nGroupDefault, nameScene, true);
            if (pScene == nullptr)
            {
                F_LogError("*********************** SceneManagerEnumerator::initSceneDefaults: Load default scene: [%s] failed !", nameScene.c_str());
                return false;
            }
            m_mapSceneDefaults[nameScene] = pScene;
            F_LogInfo("SceneManagerEnumerator::initSceneDefaults: Load default scene: [%s] success !", nameScene.c_str());
        }
        return true;
    }
    Scene* SceneManagerEnumerator::loadScene(uint32 nGroup, const String& strName, bool bIsFromFile /*= true*/)
    {   
        Scene* pScene = new Scene(nGroup, strName);
        if (!pScene->LoadScene(bIsFromFile))
        {
            F_LogError("*********************** SceneManagerEnumerator::loadScene: Load scene failed, group: [%u] name: [%s] !", nGroup, strName.c_str());
            F_DELETE(pScene)
            return nullptr;
        }
        return pScene;
    }
    void SceneManagerEnumerator::destroySceneDefaults()
    {
        for (ScenePtrMap::iterator it = m_mapSceneDefaults.begin(); 
            it != m_mapSceneDefaults.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_mapSceneDefaults.clear();
    }

	bool SceneManagerEnumerator::LoadSceneAll()
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
    Scene* SceneManagerEnumerator::LoadScene(uint32 nGroup, const String& strName)
    {
        if (m_pSceneSerializer == nullptr)
            return nullptr;

        Scene* pScene = GetScene(nGroup, strName);
        if (pScene == nullptr)
        {
            SceneInfo* pSceneInfo = m_pSceneSerializer->GetSceneInfo(nGroup, strName);
            if (pSceneInfo == nullptr)
            {
                F_LogError("*********************** SceneManagerEnumerator::LoadScene: Can not find scene info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            pScene = loadScene(pSceneInfo);
            if (!pScene)
            {
                return nullptr;
            }
        }
        return pScene;
    }
    Scene* SceneManagerEnumerator::loadScene(SceneInfo* pSI)
    {
        Scene* pScene = new Scene(pSI->group,
                                  pSI->nameScene);
        if (!pScene->LoadScene(true))
        {
            F_LogError("*********************** SceneManagerEnumerator::loadScene: Load scene failed, name: [%s], path: [%s] !", pSI->nameScene.c_str(), pSI->pathScene.c_str());
            F_DELETE(pScene)
            return nullptr;
        }

        if (AddScene(pSI->group, pScene))
        {
            F_LogInfo("SceneManagerEnumerator::loadScene: Load scene success, [%u]-[%s]-[%s] !", 
                      pSI->group, 
                      pSI->nameScene.c_str(), 
                      pSI->pathScene.c_str());
        }
        return pScene;
    }

    bool SceneManagerEnumerator::HasScene(uint32 nGroup, const String& strName)
    {
        return GetScene(nGroup, strName) != nullptr;
    }

    Scene* SceneManagerEnumerator::GetScene(uint32 nGroup, const String& strName)
    {
        Scene* pScene = GetSceneDefault(nGroup, strName);
        if (pScene != nullptr)
            return pScene;

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

    bool SceneManagerEnumerator::AddScene(uint32 nGroup, Scene* pScene)
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
            F_LogError("*********************** SceneManagerEnumerator::AddScene: Scene name already exist: [%s] !", strName.c_str());
            F_DELETE(pScene)
            return false;
        }

        itFind->second.insert(ScenePtrMap::value_type(strName, pScene));
        m_aScene.push_back(pScene);
        return true;
    }

    void SceneManagerEnumerator::DeleteScene(uint32 nGroup, const String& strName)
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

    void SceneManagerEnumerator::DeleteSceneAll()
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

}; //LostPeterEngine