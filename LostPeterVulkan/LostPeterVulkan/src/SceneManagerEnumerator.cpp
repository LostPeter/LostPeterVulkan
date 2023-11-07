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

#include "../include/SceneManagerEnumerator.h"
#include "../include/VulkanWindow.h"
#include "../include/SceneSerializer.h"
#include "../include/Scene.h"

template<> LostPeter::SceneManagerEnumerator* LostPeterFoundation::FSingleton<LostPeter::SceneManagerEnumerator>::ms_Singleton = nullptr;

namespace LostPeter
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

    SceneManagerEnumerator::SceneManagerEnumerator()
        : Base("SceneManagerEnumerator")
		, m_pSceneSerializer(nullptr)
    {

    }
    SceneManagerEnumerator::~SceneManagerEnumerator()
    {
		Destroy();
    }

	void SceneManagerEnumerator::Destroy()
	{
		F_DELETE(m_pSceneSerializer)
	}

    bool SceneManagerEnumerator::Init(uint nGroup, const String& strNameCfg)
	{	
		//1> Scene Cfg Path 
        String strPathCfgScene = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgScene.empty())
        {
            F_LogError("*********************** SceneManagerEnumerator::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Scene Serializer
        m_pSceneSerializer = new SceneSerializer();
        if (!m_pSceneSerializer->LoadFile(strPathCfgScene))
        {
            F_LogError("*********************** SceneManagerEnumerator::Init: Load file scene cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

		return true;
	}



}; //LostPeter