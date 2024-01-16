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
    F_LogInfo("++++++++++ Vulkan_018_Object::loadShaders: Load shader test start !");
    {

    }
    F_LogInfo("---------- Vulkan_018_Object::loadShaders: Load shader test end !");
}
void Vulkan_018_Object::loadMeshes()
{
    F_LogInfo("++++++++++ Vulkan_018_Object::loadMeshes: Load mesh test start !");
    {

    }
    F_LogInfo("---------- Vulkan_018_Object::loadMeshes: Load mesh test end !");
}
void Vulkan_018_Object::loadTextures()
{
    F_LogInfo("++++++++++ Vulkan_018_Object::loadTextures: Load texture test start !");
    {
        // 6001 - default_blackwhite
        uint nGroup = 6001;
        String strNameTexture = "default_blackwhite";
        Texture* pTexture1 = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
        if (pTexture1 == nullptr)
        {
            F_LogError("*********************** Vulkan_018_Object::loadTextures: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
            return;
        }
        F_LogInfo("Vulkan_018_Object::loadTextures: Load textue success, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());

        // 6002 - mountain_diffuse
        nGroup = 6002;
        strNameTexture = "mountain_diffuse";
        Texture* pTexture2 = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
        if (pTexture2 == nullptr)
        {
            F_LogError("*********************** Vulkan_018_Object::loadTextures: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
            return;
        }
        F_LogInfo("Vulkan_018_Object::loadTextures: Load textue success, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());

        //6004 - texture_terrain_diffuse
        // nGroup = 6004;
        // strNameTexture = "texture_terrain_diffuse";
        // Texture* pTexture3 = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
        // if (pTexture3 == nullptr)
        // {
        //     F_LogError("*********************** Vulkan_018_Object::loadTextures: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
        //     return;
        // }
        // F_LogInfo("Vulkan_018_Object::loadTextures: Load textue success, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());

        //6006 - texturecubemap
        // nGroup = 6006;
        // strNameTexture = "texturecubemap";
        // Texture* pTexture4 = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
        // if (pTexture4 == nullptr)
        // {
        //     F_LogError("*********************** Vulkan_018_Object::loadTextures: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
        //     return;
        // }
        // F_LogInfo("Vulkan_018_Object::loadTextures: Load textue success, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());

    }
    F_LogInfo("---------- Vulkan_018_Object::loadTextures: Load texture test end !");
}
void Vulkan_018_Object::loadMaterials()
{
    F_LogInfo("++++++++++ Vulkan_018_Object::loadMaterials: Load material test start !");
    {

    }
    F_LogInfo("---------- Vulkan_018_Object::loadMaterials: Load material test end !");
}