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

namespace LostPeter
{
    #define DESCRIPTOR_SET_LAYOUT_TAG_DESCRIPTOR_SET_LAYOUT_CFG					    "cfg_descriptor_set_layout"
    #define	DESCRIPTOR_SET_LAYOUT_TAG_DESCRIPTOR_SET_LAYOUT							"descriptor_set_layout"

#define DESCRIPTOR_SET_LAYOUT_TAG_ATTRIBUTE_NAME		            "name"


    VKDescriptorSetLayoutSerializer::VKDescriptorSetLayoutSerializer()
        : Base("VKDescriptorSetLayoutSerializer")
    {

    }
    VKDescriptorSetLayoutSerializer::~VKDescriptorSetLayoutSerializer()
    {
        DeleteDescriptorSetLayoutInfoAll();
    }
    
    bool VKDescriptorSetLayoutSerializer::HasDescriptorSetLayoutInfo(const String& strName)
    {
        return GetDescriptorSetLayoutInfo(strName) != nullptr;
    }
    DescriptorSetLayoutInfo* VKDescriptorSetLayoutSerializer::GetDescriptorSetLayoutInfo(const String& strName)
    {
        DescriptorSetLayoutInfoPtrMap::iterator itFind = m_mapDescriptorSetLayoutInfo.find(strName);
        if (itFind == m_mapDescriptorSetLayoutInfo.end())
            return nullptr;
        return itFind->second;
    }
    bool VKDescriptorSetLayoutSerializer::AddDescriptorSetLayoutInfo(DescriptorSetLayoutInfo* pDescriptorSetLayoutInfo)
    {
        DeleteDescriptorSetLayoutInfo(pDescriptorSetLayoutInfo->nameDescriptorSetLayoutInfo);

        m_mapDescriptorSetLayoutInfo.insert(DescriptorSetLayoutInfoPtrMap::value_type(pDescriptorSetLayoutInfo->nameDescriptorSetLayoutInfo, pDescriptorSetLayoutInfo));
        m_aDescriptorSetLayoutInfo.push_back(pDescriptorSetLayoutInfo);
        return true;
    }
    void VKDescriptorSetLayoutSerializer::DeleteDescriptorSetLayoutInfo(const String& strName)
    {
        DescriptorSetLayoutInfoPtrMap::iterator itFind = m_mapDescriptorSetLayoutInfo.find(strName);
        if (itFind == m_mapDescriptorSetLayoutInfo.end())
        {
            return;
        }
        DescriptorSetLayoutInfoPtrVector::iterator itFindA = std::find(m_aDescriptorSetLayoutInfo.begin(), m_aDescriptorSetLayoutInfo.end(), itFind->second);
        if (itFindA != m_aDescriptorSetLayoutInfo.end())
            m_aDescriptorSetLayoutInfo.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapDescriptorSetLayoutInfo.erase(itFind);
    }
    void VKDescriptorSetLayoutSerializer::DeleteDescriptorSetLayoutInfoAll()
    {
        for (DescriptorSetLayoutInfoPtrMap::iterator it = m_mapDescriptorSetLayoutInfo.begin();
             it != m_mapDescriptorSetLayoutInfo.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aDescriptorSetLayoutInfo.clear();
        m_mapDescriptorSetLayoutInfo.clear();
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

        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameDescriptorSetLayout;
            if (!pChild->ParserAttribute_String(DESCRIPTOR_SET_LAYOUT_TAG_ATTRIBUTE_NAME, strNameDescriptorSetLayout))
            {
                F_LogError("*********************** VKDescriptorSetLayoutSerializer::deserializeXML: Can not find attribute: 'name', descriptor set layout index: [%d] !", i);
                continue;
            }

            DescriptorSetLayoutInfo* pDescriptorSetLayoutInfo = new DescriptorSetLayoutInfo(strNameDescriptorSetLayout);
            if (AddDescriptorSetLayoutInfo(pDescriptorSetLayoutInfo))
            {
                F_LogInfo("VKDescriptorSetLayoutSerializer::deserializeXML: Add descriptor set layout info success, [%s] !",
                          strNameDescriptorSetLayout.c_str());
            }
        }

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