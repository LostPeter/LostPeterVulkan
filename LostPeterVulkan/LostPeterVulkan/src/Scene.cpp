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

}; //LostPeter