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

#include "../include/SceneSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    SceneSerializer::SceneSerializer()
        : Base("SceneSerializer")
    {
        
    }
    SceneSerializer::~SceneSerializer()
    {

    }
    
    //File Content XML
    bool SceneSerializer::serializeXML()
    {
        return false;
    }
    bool SceneSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool SceneSerializer::serializeBinary()
    {
        return false;
    }
    bool SceneSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter