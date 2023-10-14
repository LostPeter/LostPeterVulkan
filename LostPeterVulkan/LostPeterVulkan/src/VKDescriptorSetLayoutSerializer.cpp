/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKDescriptorSetLayoutSerializer.h"
#include "../include/VulkanWindow.h"
#include "../include/FUtilString.h"

namespace LostPeter
{
    #define DESCRIPTOR_TAG_DESCRIPTOR_CFG					    "cfg_descriptor"
    #define	DESCRIPTOR_TAG_DESCRIPTOR							"descriptor"

#define DESCRIPTOR_TAG_ATTRIBUTE_NAME		            "name"
#define DESCRIPTOR_TAG_ATTRIBUTE_TYPE			        "type"


    VKDescriptorSetLayoutSerializer::VKDescriptorSetLayoutSerializer()
        : Base("VKDescriptorSetLayoutSerializer")
    {

    }
    VKDescriptorSetLayoutSerializer::~VKDescriptorSetLayoutSerializer()
    {

    }
    
    void VKDescriptorSetLayoutSerializer::Destroy()
    {
        
    }

    //File Content XML
    bool VKDescriptorSetLayoutSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKDescriptorSetLayoutSerializer::serializeXML")


        return false;
    }
    bool VKDescriptorSetLayoutSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKDescriptorSetLayoutSerializer::deserializeXML")

        return true;
    }

    //File Content Binary
    bool VKDescriptorSetLayoutSerializer::serializeBinary()
    {
        return false;
    }
    bool VKDescriptorSetLayoutSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter