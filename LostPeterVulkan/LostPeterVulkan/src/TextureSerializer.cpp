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

#include "../include/TextureSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    TextureSerializer::TextureSerializer()
        : Base("TextureSerializer")
    {
        
    }
    TextureSerializer::~TextureSerializer()
    {

    }
    
    //File Content XML
    bool TextureSerializer::serializeXML()
    {
        return false;
    }
    bool TextureSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool TextureSerializer::serializeBinary()
    {
        return false;
    }
    bool TextureSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter