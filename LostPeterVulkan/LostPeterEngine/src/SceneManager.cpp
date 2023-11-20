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

#include "../include/SceneManager.h"
#include "../include/Scene.h"
#include "../include/SceneNode.h"
#include "../include/Viewport.h"
#include "../include/ObjectCamera.h"


namespace LostPeterEngine
{
    SceneManager::SceneManager(const String& nameSceneManager, Scene* pScene)
        : Base(nameSceneManager)
        , m_pScene(pScene)
    {

    }
    SceneManager::~SceneManager()
    {
        Destroy();
    }

    void SceneManager::Destroy()
    {

    }

    bool SceneManager::Init()
    {
        
        return true;
    }

    void SceneManager::UpdateScene(Viewport* pViewport)
    {
        
    }

    void SceneManager::RenderScene(Scene* pScene)
    {

    }

}; //LostPeterEngine