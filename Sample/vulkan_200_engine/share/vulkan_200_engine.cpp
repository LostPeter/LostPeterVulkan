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
#include "vulkan_200_engine.h"


Vulkan_200_Engine::Vulkan_200_Engine(const String& nameSample, int width, int height)
    : Sample(nameSample, width, height)
{
    m_cfg_bIsWindowSceneUsed = true;
    m_cfg_bIsWindowGameUsed = false;
}


void Vulkan_200_Engine::OnLoad()
{   
    loadShaders();
    loadMeshes();
    loadTextures();
    loadMaterials();
}

void Vulkan_200_Engine::loadShaders()
{
    F_LogInfo("++++++++++ Vulkan_200_Engine::loadShaders: Load shader test start !");
    {
        uint nGroup = 4000;

        //4000 - vert_default
        ShaderProgram* pSP_VertDefault = loadShader(nGroup, "vert_default");

        //4000 - frag_default
        ShaderProgram* pSP_FragDefault = loadShader(nGroup, "frag_default");

        //4000 - vert_default_opaque
        ShaderProgram* pSP_VertDefaultOpaque = loadShader(nGroup, "vert_default_opaque");

        //4000 - frag_default_opaque
        ShaderProgram* pSP_FragDefaultOpaque = loadShader(nGroup, "frag_default_opaque");

        //4000 - vert_default_transparent
        ShaderProgram* pSP_VertDefaultTransparent = loadShader(nGroup, "vert_default_transparent");

        //4000 - frag_default_transparent
        ShaderProgram* pSP_FragDefaultTransparent = loadShader(nGroup, "frag_default_transparent");
    }
    F_LogInfo("---------- Vulkan_200_Engine::loadShaders: Load shader test end !");
}
    ShaderProgram* Vulkan_200_Engine::loadShader(uint nGroup, const String& strNameShaderProgram)
    {
        ShaderProgram* pShaderProgram = ShaderProgramManager::GetSingleton().LoadShaderProgram(nGroup, strNameShaderProgram);
        if (pShaderProgram == nullptr)
        {
            F_LogError("*********************** Vulkan_200_Engine::loadShader: Load shader program failed, group: [%u], name: [%s] !", nGroup, strNameShaderProgram.c_str());
            return nullptr;
        }
        F_LogInfo("Vulkan_200_Engine::loadShader: Load shader program success, group: [%u], name: [%s], path: [%s], type: [%s] !", 
                  nGroup, strNameShaderProgram.c_str(), pShaderProgram->GetPath().c_str(), 
                  F_GetShaderTypeName(pShaderProgram->GetShaderType()).c_str());
        return pShaderProgram;
    }

void Vulkan_200_Engine::loadMeshes()
{
    F_LogInfo("++++++++++ Vulkan_200_Engine::loadMeshes: Load mesh test start !");
    {
        uint nGroup = 5001;

        //5001 - plane
        String strNameMesh = "plane";
        Mesh* pMesh1 = loadMesh(nGroup, strNameMesh);

        // //5001 - cube
        // strNameMesh = "cube";
        // Mesh* pMesh2 = loadMesh(nGroup, strNameMesh);

        // //5001 - sphere
        // strNameMesh = "sphere";
        // Mesh* pMesh3 = loadMesh(nGroup, strNameMesh);

        // nGroup = 5002;
        // //5002 - mountain
        // strNameMesh = "mountain";
        // Mesh* pMesh4 = loadMesh(nGroup, strNameMesh);

        // //5002 - rock
        // strNameMesh = "rock";
        // Mesh* pMesh5 = loadMesh(nGroup, strNameMesh);

        // //5002 - cliff
        // strNameMesh = "cliff";
        // Mesh* pMesh6 = loadMesh(nGroup, strNameMesh);

        // //5002 - tree
        // strNameMesh = "tree";
        // Mesh* pMesh7 = loadMesh(nGroup, strNameMesh);

        // //5002 - tree_spruce
        // strNameMesh = "tree_spruce";
        // Mesh* pMesh8 = loadMesh(nGroup, strNameMesh);

        // //5002 - grass
        // strNameMesh = "grass";
        // Mesh* pMesh9 = loadMesh(nGroup, strNameMesh);

        // //5002 - flower
        // strNameMesh = "flower";
        // Mesh* pMesh10 = loadMesh(nGroup, strNameMesh);
    }
    F_LogInfo("---------- Vulkan_200_Engine::loadMeshes: Load mesh test end !");
}
    Mesh* Vulkan_200_Engine::loadMesh(uint nGroup, const String& strNameMesh)
    {
        Mesh* pMesh = MeshManager::GetSingleton().LoadMesh(nGroup, strNameMesh);
        if (pMesh == nullptr)
        {
            F_LogError("*********************** Vulkan_200_Engine::loadMesh: Load mesh failed, group: [%u], name: [%s] !", nGroup, strNameMesh.c_str());
            return nullptr;
        }
        F_LogInfo("Vulkan_200_Engine::loadMesh: Load mesh success, group: [%u], name: [%s], path: [%s], type: [%s] !", 
                  nGroup, strNameMesh.c_str(), pMesh->GetPath().c_str(), 
                  F_GetMeshTypeName(pMesh->GetMeshType()).c_str());
        return pMesh;
    }

void Vulkan_200_Engine::loadTextures()
{
    F_LogInfo("++++++++++ Vulkan_200_Engine::loadTextures: Load texture test start !");
    {
        //6001 - default_blackwhite
        uint nGroup = 6001;
        String strNameTexture = "default_blackwhite";
        Texture* pTexture1 = loadTexture(nGroup, strNameTexture, false);

        //6002 - mountain_diffuse
        nGroup = 6002;
        strNameTexture = "mountain_diffuse";
        Texture* pTexture2 = loadTexture(nGroup, strNameTexture, false);

        //6004 - texture_terrain_diffuse
        nGroup = 6004;
        strNameTexture = "texture_terrain_diffuse";
        Texture* pTexture3 = loadTexture(nGroup, strNameTexture, true);

        //6006 - texturecubemap
        nGroup = 6006;
        strNameTexture = "texturecubemap";
        Texture* pTexture4 = loadTexture(nGroup, strNameTexture, true);
    }
    F_LogInfo("---------- Vulkan_200_Engine::loadTextures: Load texture test end !");
}
    Texture* Vulkan_200_Engine::loadTexture(uint nGroup, const String& strNameTexture, bool bIsArray)
    {
        Texture* pTexture = TextureManager::GetSingleton().LoadTexture(nGroup, strNameTexture);
        if (pTexture == nullptr)
        {
            F_LogError("*********************** Vulkan_200_Engine::loadTexture: Load textue failed, group: [%u], name: [%s] !", nGroup, strNameTexture.c_str());
            return nullptr;
        }
        if (bIsArray)
        {
            F_LogInfo("Vulkan_200_Engine::loadTexture: Load textue success, group: [%u], name: [%s], path: [%s][%s][%s][%s], formatDesire: [%s], formatSrc: [%s] !", 
                      nGroup, strNameTexture.c_str(),
                      pTexture->GetPath().at(0).c_str(), 
                      pTexture->GetPath().at(1).c_str(),
                      pTexture->GetPath().at(2).c_str(),
                      pTexture->GetPath().at(3).c_str(),
                      FPixelFormat::GetPixelFormatName(pTexture->GetPixelFormatDesired()).c_str(),
                      FPixelFormat::GetPixelFormatName(pTexture->GetPixelFormatSrc()).c_str());
        }
        else
        {
            F_LogInfo("Vulkan_200_Engine::loadTexture: Load textue success, group: [%u], name: [%s], path: [%s], formatDesire: [%s], formatSrc: [%s] !", 
                      nGroup, strNameTexture.c_str(), pTexture->GetPath().at(0).c_str(),
                      FPixelFormat::GetPixelFormatName(pTexture->GetPixelFormatDesired()).c_str(),
                      FPixelFormat::GetPixelFormatName(pTexture->GetPixelFormatSrc()).c_str());
        }
        return pTexture;
    }
void Vulkan_200_Engine::loadMaterials()
{
    F_LogInfo("++++++++++ Vulkan_200_Engine::loadMaterials: Load material test start !");
    {

    }
    F_LogInfo("---------- Vulkan_200_Engine::loadMaterials: Load material test end !");
}
    Material* Vulkan_200_Engine::loadMaterial(uint nGroup, const String& strNameMaterial)
    {
        return nullptr;
    }