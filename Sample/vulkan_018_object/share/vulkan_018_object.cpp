/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-08-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_018_object.h"


Vulkan_018_Object::Vulkan_018_Object(const String& nameSample, int width, int height)
    : Sample(nameSample, width, height)
{
    m_cfg_bIsWindowSceneUsed = true;
    m_cfg_bIsWindowGameUsed = false;
}


void Vulkan_018_Object::OnLoad()
{   
    loadShaders();
    loadMeshes();
    loadTextures();
    loadMaterials();
}

void Vulkan_018_Object::loadShaders()
{

    F_LogInfo("Vulkan_018_Object::loadShaders: Load shader success !");
}
void Vulkan_018_Object::loadMeshes()
{
    
    F_LogInfo("Vulkan_018_Object::loadMeshes: Load mesh success !");
}
void Vulkan_018_Object::loadTextures()
{
    // uint nGroup = 6001;
    // String strNameTexture = "default_blackwhite";
    // Texture* pTexture = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
    // if (pTexture == nullptr)
    // {
    //     F_LogError("*********************** Vulkan_018_Object::loadTextures: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
    //     return;
    // }

    F_LogInfo("Vulkan_018_Object::loadTextures: Load texture success !");
}
void Vulkan_018_Object::loadMaterials()
{

    F_LogInfo("Vulkan_018_Object::loadMaterials: Load material success !");
}