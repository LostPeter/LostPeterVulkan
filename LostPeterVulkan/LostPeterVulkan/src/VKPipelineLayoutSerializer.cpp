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

#include "../include/VKPipelineLayoutSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    #define PIPELINE_LAYOUT_TAG_PIPELINE_LAYOUT_CFG					    "cfg_pipeline_layout"
    #define	PIPELINE_LAYOUT_TAG_PIPELINE_LAYOUT							"pipeline_layout"

#define PIPELINE_LAYOUT_TAG_ATTRIBUTE_NAME		            "name"
#define PIPELINE_LAYOUT_TAG_ATTRIBUTE_TYPE			        "type"


    VKPipelineLayoutSerializer::VKPipelineLayoutSerializer()
        : Base("VKPipelineLayoutSerializer")
    {

    }
    VKPipelineLayoutSerializer::~VKPipelineLayoutSerializer()
    {

    }
    
    void VKPipelineLayoutSerializer::Destroy()
    {
        
    }

    //File Content XML
    bool VKPipelineLayoutSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKPipelineLayoutSerializer::serializeXML")


        return false;
    }
    bool VKPipelineLayoutSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKPipelineLayoutSerializer::deserializeXML")

        return true;
    }

    //File Content Binary
    bool VKPipelineLayoutSerializer::serializeBinary()
    {
        return false;
    }
    bool VKPipelineLayoutSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter