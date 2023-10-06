/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    MaterialSerializer::MaterialSerializer()
        : Base("MaterialSerializer")
    {
        
    }
    MaterialSerializer::~MaterialSerializer()
    {

    }
    
    //File Content XML
    bool MaterialSerializer::serializeXML()
    {
        return false;
    }
    bool MaterialSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool MaterialSerializer::serializeBinary()
    {
        return false;
    }
    bool MaterialSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter