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

#include "../include/VKDescriptorSetSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    #define DESCRIPTOR_SET_TAG_DESCRIPTOR_SET_CFG					    "cfg_descriptor_set"
    #define	DESCRIPTOR_SET_TAG_DESCRIPTOR_SET							"descriptor_set"

#define DESCRIPTOR_SET_TAG_ATTRIBUTE_NAME		            "name"
#define DESCRIPTOR_SET_TAG_ATTRIBUTE_SET_USAGE			    "set_usage"
#define DESCRIPTOR_SET_TAG_ATTRIBUTE_SHADER_USAGE			"shader_usage"


    VKDescriptorSetSerializer::VKDescriptorSetSerializer()
        : Base("VKDescriptorSetSerializer")
    {

    }
    VKDescriptorSetSerializer::~VKDescriptorSetSerializer()
    {
        DeleteDescriptorSetInfoAll();
    }
    
    bool VKDescriptorSetSerializer::HasDescriptorSetInfo(const String& strName)
    {
        return GetDescriptorSetInfo(strName) != nullptr;
    }
    DescriptorSetInfo* VKDescriptorSetSerializer::GetDescriptorSetInfo(const String& strName)
    {
        DescriptorSetInfoPtrMap::iterator itFind = m_mapDescriptorSetInfo.find(strName);
        if (itFind == m_mapDescriptorSetInfo.end())
            return nullptr;
        return itFind->second;
    }
    bool VKDescriptorSetSerializer::AddDescriptorSetInfo(DescriptorSetInfo* pDescriptorSetInfo)
    {
        DeleteDescriptorSetInfo(pDescriptorSetInfo->nameDescriptorSetInfo);

        m_mapDescriptorSetInfo.insert(DescriptorSetInfoPtrMap::value_type(pDescriptorSetInfo->nameDescriptorSetInfo, pDescriptorSetInfo));
        m_aDescriptorSetInfo.push_back(pDescriptorSetInfo);
        return true;
    }
    void VKDescriptorSetSerializer::DeleteDescriptorSetInfo(const String& strName)
    {
        DescriptorSetInfoPtrMap::iterator itFind = m_mapDescriptorSetInfo.find(strName);
        if (itFind == m_mapDescriptorSetInfo.end())
        {
            return;
        }
        DescriptorSetInfoPtrVector::iterator itFindA = std::find(m_aDescriptorSetInfo.begin(), m_aDescriptorSetInfo.end(), itFind->second);
        if (itFindA != m_aDescriptorSetInfo.end())
            m_aDescriptorSetInfo.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapDescriptorSetInfo.erase(itFind);
    }
    void VKDescriptorSetSerializer::DeleteDescriptorSetInfoAll()
    {
        for (DescriptorSetInfoPtrMap::iterator it = m_mapDescriptorSetInfo.begin();
             it != m_mapDescriptorSetInfo.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aDescriptorSetInfo.clear();
        m_mapDescriptorSetInfo.clear();
    }

    //File Content XML
    bool VKDescriptorSetSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKDescriptorSetSerializer::serializeXML")

        return false;
    }
    bool VKDescriptorSetSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "VKDescriptorSetSerializer::deserializeXML")

        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameDescriptorSet;
            if (!pChild->ParserAttribute_String(DESCRIPTOR_SET_TAG_ATTRIBUTE_NAME, strNameDescriptorSet))
            {
                F_LogError("*********************** VKDescriptorSetSerializer::deserializeXML: Can not find attribute: 'name', descriptor set index: [%d] !", i);
                continue;
            }
            VulkanDescriptorSetType typeDescriptorSet = Util_ParseDescriptorSetType(strNameDescriptorSet);
            //set_usage
            String strNameSetUsage;
            if (!pChild->ParserAttribute_String(DESCRIPTOR_SET_TAG_ATTRIBUTE_SET_USAGE, strNameSetUsage))
            {
                F_LogError("*********************** VKDescriptorSetSerializer::deserializeXML: Can not find attribute: 'set_usage', descriptor set index: [%d] !", i);
                continue;
            }
            VulkanDescriptorSetUsageType typeDescriptorSetUsage = Util_ParseDescriptorSetUsageType(strNameSetUsage);
            //shader_usage
            String strNameShaderUsage;
            if (!pChild->ParserAttribute_String(DESCRIPTOR_SET_TAG_ATTRIBUTE_SHADER_USAGE, strNameShaderUsage))
            {
                F_LogError("*********************** VKDescriptorSetSerializer::deserializeXML: Can not find attribute: 'shader_usage', descriptor set index: [%d] !", i);
                continue;
            }

            DescriptorSetInfo* pDescriptorSetInfo = new DescriptorSetInfo(strNameDescriptorSet,
                                                                          typeDescriptorSet,
                                                                          typeDescriptorSetUsage,
                                                                          strNameShaderUsage);
            if (AddDescriptorSetInfo(pDescriptorSetInfo))
            {
                F_LogInfo("VKDescriptorSetSerializer::deserializeXML: Add descriptor set info success, [%s]-[%s]-[%s] !",
                          strNameDescriptorSet.c_str(), 
                          strNameSetUsage.c_str(), 
                          strNameShaderUsage.c_str());
            }
        }

        return true;
    }

    //File Content Binary
    bool VKDescriptorSetSerializer::serializeBinary()
    {
        return false;
    }
    bool VKDescriptorSetSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter