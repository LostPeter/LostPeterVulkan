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

#include "../include/MaterialDataSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{


    
    MaterialDataSerializer::MaterialDataSerializer()
        : Base("MaterialDataSerializer")
    {
        
    }
    MaterialDataSerializer::~MaterialDataSerializer()
    {

    }
    
    //File Content XML
    bool MaterialDataSerializer::serializeXML()
    {
        return false;
    }
    bool MaterialDataSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool MaterialDataSerializer::serializeBinary()
    {
        return false;
    }
    bool MaterialDataSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter