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

namespace LostPeter
{
    Scene::Scene(const String& nameScene)
        : Base(nameScene)
    ////Viewport
        , m_pViewportMain(nullptr)
    ////SceneNode
        , m_pSceneNodeRoot(nullptr)
    ////Object
        
    {

    }
    Scene::~Scene()
    {
        Destroy();
    }

    void Scene::Destroy()
    {

    }

    bool Scene::Init()
    {

        return true;
    }

}; //LostPeter