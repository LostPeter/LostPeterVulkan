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
#include "../include/FUtilString.h"

namespace LostPeter
{
    #define TEXTURE_TAG_TEXTURE_CFG								"cfg_texture"
    #define	TEXTURE_TAG_TEXTURE									"texture"




    TextureSerializer::TextureSerializer()
        : Base("TextureSerializer")
    {
        
    }
    TextureSerializer::~TextureSerializer()
    {
        DeleteTextureInfoAll();
    }
    

    bool TextureSerializer::HasTextureInfo(uint nGroup, const String& strName)
    {
        return GetTextureInfo(nGroup, strName) != nullptr;
    }
    TextureInfo* TextureSerializer::GetTextureInfo(uint nGroup, const String& strName)
    {
        TextureInfoGroupPtrMap::iterator itFind = m_mapTextureInfoGroup.find(nGroup);
        if (itFind == m_mapTextureInfoGroup.end())
            return nullptr;
        TextureInfoPtrMap::iterator itFindTI = itFind->second.find(strName);
        if (itFindTI == itFind->second.end())
            return nullptr;
        return itFindTI->second;
    }
    bool TextureSerializer::AddTextureInfo(TextureInfo* pTextureInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Texture, pTextureInfo->pathTexture, nGroup))
        {   
            F_LogError("*********************** TextureSerializer::AddTextureInfo: Can not find group by texture path: [%s] !", pTextureInfo->pathTexture.c_str());
            F_DELETE(pTextureInfo)
            return false;
        }
        
        TextureInfoGroupPtrMap::iterator itFind = m_mapTextureInfoGroup.find(nGroup);
        if (itFind == m_mapTextureInfoGroup.end())
        {
            TextureInfoPtrMap mapTIs;
            m_mapTextureInfoGroup[nGroup] = mapTIs;
            itFind = m_mapTextureInfoGroup.find(nGroup);
        }
        TextureInfoPtrMap::iterator itFindTI = itFind->second.find(pTextureInfo->nameTexture);
        if (itFindTI != itFind->second.end())
        {
            F_LogError("*********************** TextureSerializer::AddTextureInfo: Texture name already exist: [%s] !", pTextureInfo->nameTexture.c_str());
            F_DELETE(pTextureInfo)
            return false;
        }

        pTextureInfo->group = nGroup;
        itFind->second.insert(TextureInfoPtrMap::value_type(pTextureInfo->nameTexture, pTextureInfo));
        m_aTextureInfo.push_back(pTextureInfo);
        return true;
    }
    void TextureSerializer::DeleteTextureInfo(uint nGroup, const String& strName)
    {
        TextureInfoGroupPtrMap::iterator itFind = m_mapTextureInfoGroup.find(nGroup);
        if (itFind == m_mapTextureInfoGroup.end())
        {
            return;
        }

        TextureInfoPtrMap::iterator itFindTI = itFind->second.find(strName);
        if (itFindTI != itFind->second.end())
        {
            TextureInfoPtrVector::iterator itFindA = std::find(m_aTextureInfo.begin(), m_aTextureInfo.end(), itFindTI->second);
            if (itFindA != m_aTextureInfo.end())
                m_aTextureInfo.erase(itFindA);
            F_DELETE(itFindTI->second)
            itFind->second.erase(itFindTI);
        }
    }
    void TextureSerializer::DeleteTextureInfoAll()
    {
        for (TextureInfoGroupPtrMap::iterator it = m_mapTextureInfoGroup.begin();
             it != m_mapTextureInfoGroup.end(); ++it)
        {
            TextureInfoPtrMap& mapTIs = it->second;
            for (TextureInfoPtrMap::iterator itTI = mapTIs.begin(); 
                 itTI != mapTIs.end(); ++itTI)
            {
                F_DELETE(itTI->second)
            }
        }
        m_aTextureInfo.clear();
        m_mapTextureInfoGroup.clear();
    }

    //File Content XML
    bool TextureSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "TextureSerializer::serializeXML")

        return false;
    }
    bool TextureSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "TextureSerializer::deserializeXML")

        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);
            
        }

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