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

#include "../include/ObjectSkinMesh.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    ObjectSkinMesh::ObjectSkinMesh(const String& nameSkinMesh)
        : Object(nameSkinMesh)
    {
        this->typeObject = Vulkan_Object_SkinMesh;
    }
    ObjectSkinMesh::~ObjectSkinMesh()
    {

    }

}; //LostPeter