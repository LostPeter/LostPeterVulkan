/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MeshDataSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    MeshDataSerializer::MeshDataSerializer()
        : Base("MeshDataSerializer")
    {
        
    }
    MeshDataSerializer::~MeshDataSerializer()
    {

    }
    
    //File Content XML
    bool MeshDataSerializer::serializeXML()
    {
        return false;
    }
    bool MeshDataSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool MeshDataSerializer::serializeBinary()
    {
        return false;
    }
    bool MeshDataSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter