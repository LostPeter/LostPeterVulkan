/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneDataManager.h"
#include "../include/VulkanWindow.h"
#include "../include/SceneDataSerializer.h"

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
    {
        m_pSceneDataSerializer = new SceneDataSerializer(this);
    }
    SceneDataManager::~SceneDataManager()
    {
        Destroy();
    }

    void SceneDataManager::Destroy()
    {

    }

    

}; //LostPeter