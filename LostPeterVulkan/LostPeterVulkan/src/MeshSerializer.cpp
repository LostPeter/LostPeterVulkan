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

#include "../include/MeshSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    MeshSerializer::MeshSerializer()
        : Base("MeshSerializer")
    {
        
    }
    MeshSerializer::~MeshSerializer()
    {

    }
    
    //File Content XML
    bool MeshSerializer::serializeXML()
    {
        return false;
    }
    bool MeshSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool MeshSerializer::serializeBinary()
    {
        return false;
    }
    bool MeshSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter