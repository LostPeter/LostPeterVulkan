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

#ifndef _VULKAN_200_ENGINE_H_
#define _VULKAN_200_ENGINE_H_

#include "Sample.h"
using namespace LostPeterEngine;

class Vulkan_200_Engine : public Sample
{
public:
    Vulkan_200_Engine(const String& nameSample, int width, int height);

public: 
    

public:
    virtual void OnLoad();

protected:
    virtual void loadShaders();
        ShaderProgram* loadShader(uint nGroup, const String& strNameShaderProgram);
    virtual void loadMeshes();
        Mesh* loadMesh(uint nGroup, const String& strNameMesh);
    virtual void loadTextures();
        Texture* loadTexture(uint nGroup, const String& strNameTexture, bool bIsArray);
    virtual void loadMaterials();
        Material* loadMaterial(uint nGroup, const String& strNameMaterial);
    virtual void loadScenes();
        Scene* loadScene(uint nGroup, const String& strNameScene);
};


#endif