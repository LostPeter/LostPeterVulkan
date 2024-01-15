/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TextureSerializer.h"

namespace LostPeterEngine
{
    #define TEXTURE_TAG_TEXTURE_CFG								"cfg_texture"
    #define	TEXTURE_TAG_TEXTURE									"texture"

#define TEXTURE_TAG_ATTRIBUTE_NAME		            "name"
#define TEXTURE_TAG_ATTRIBUTE_PATH			        "path"
#define TEXTURE_TAG_ATTRIBUTE_TYPE			        "type"
#define TEXTURE_TAG_ATTRIBUTE_FORMAT			    "format"
#define TEXTURE_TAG_ATTRIBUTE_FILTER			    "filter"
#define TEXTURE_TAG_ATTRIBUTE_ADDRESSING			"addressing"
#define TEXTURE_TAG_ATTRIBUTE_BORDER_COLOR			"border_color"
#define TEXTURE_TAG_ATTRIBUTE_SAMPLE_COUNT			"sample_count"
#define TEXTURE_TAG_ATTRIBUTE_SIZE			        "size"
#define TEXTURE_TAG_ATTRIBUTE_ANIM_CHUNK			"anim_chunk"
#define TEXTURE_TAG_ATTRIBUTE_IS_ALPHA			    "is_alpha"
#define TEXTURE_TAG_ATTRIBUTE_IS_RT			        "is_rt"
#define TEXTURE_TAG_ATTRIBUTE_IS_GCS			    "is_gcs"


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
            
            //name
            String strNameTexture;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_NAME, strNameTexture))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'name', texture index: [%d] !", i);
                continue;
            }
            //path
            String strPathTexture;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_PATH, strPathTexture))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'path', texture index: [%d] !", i);
                continue;
            }
            //type
            String strType;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_TYPE, strType))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'type', texture index: [%d] !", i);
                continue;
            }
            FTextureType typeTexture = F_ParseTextureType(strType);
            //format
            String strFormat;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_FORMAT, strFormat))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'format', texture index: [%d] !", i);
                continue;
            }
            FPixelFormatType typePixelFormat = FPixelFormat::ParsePixelFormatFromName(strFormat);
            //filter
            String strFilter;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_FILTER, strFilter))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'filter', texture index: [%d] !", i);
                continue;
            }
            FTextureFilterType typeTextureFilter = F_ParseTextureFilterType(strFilter);
            //addressing
            String strAddressing;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_ADDRESSING, strAddressing))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'addressing', texture index: [%d] !", i);
                continue;
            }
            FTextureAddressingType typeTextureAddressing = F_ParseTextureAddressingType(strAddressing);
            //border_color
            String strBorderColor;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_BORDER_COLOR, strBorderColor))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'border_color', texture index: [%d] !", i);
                continue;
            }
            FTextureBorderColorType typeTextureBorderColor = F_ParseTextureBorderColorType(strBorderColor);
            //sample_count
            String strSampleCount;
            if (!pChild->ParserAttribute_String(TEXTURE_TAG_ATTRIBUTE_SAMPLE_COUNT, strSampleCount))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'sample_count', texture index: [%d] !", i);
                continue;
            }
            FMSAASampleCountType typeMSAASampleCount = F_ParseMSAASampleCountType(strSampleCount);
            //size
            FVector3 vSize;
            if (!pChild->ParserAttribute_Vector3(TEXTURE_TAG_ATTRIBUTE_SIZE, vSize))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'size', texture index: [%d] !", i);
                continue;
            }
            int width = (int)vSize.x;
            int height = (int)vSize.y;
            int depth = (int)vSize.z;
            //anim_chunk
            FVector2 vAnimChunk;
            if (!pChild->ParserAttribute_Vector2(TEXTURE_TAG_ATTRIBUTE_ANIM_CHUNK, vAnimChunk))
            {
                F_LogError("*********************** TextureSerializer::deserializeXML: Can not find attribute: 'anim_chunk', texture index: [%d] !", i);
                continue;
            }
            int animChunkX = (int)vAnimChunk.x;
            int animChunkY = (int)vAnimChunk.y;
            //is_alpha
            bool isAlpha = false;
            pChild->ParserAttribute_Bool(TEXTURE_TAG_ATTRIBUTE_IS_ALPHA, isAlpha);
            //is_rt
            bool isRT = false;
            pChild->ParserAttribute_Bool(TEXTURE_TAG_ATTRIBUTE_IS_RT, isRT);
            //is_gcs
            bool isGCS = false;
            pChild->ParserAttribute_Bool(TEXTURE_TAG_ATTRIBUTE_IS_GCS, isGCS);


            TextureInfo* pTextureInfo = new TextureInfo(strNameTexture,
                                                        strPathTexture,
                                                        typeTexture,
                                                        typePixelFormat,
                                                        typeTextureFilter,
                                                        typeTextureAddressing,
                                                        typeTextureBorderColor,
                                                        typeMSAASampleCount,
                                                        width, height, depth,
                                                        animChunkX, animChunkY,
                                                        isAlpha,
                                                        isRT,
                                                        isGCS);
            if (AddTextureInfo(pTextureInfo))
            {
                F_LogInfo("TextureSerializer::deserializeXML: Add texture info success, [%s]-[%s]-[%s]-[%s]-[%s]-[%s]-[%s]-[%s]-[%d-%d-%d]-[%d-%d]-[%s]-[%s]-[%s] !",
                          strNameTexture.c_str(), 
                          strPathTexture.c_str(), 
                          strType.c_str(), 
                          strFormat.c_str(), 
                          strFilter.c_str(), 
                          strAddressing.c_str(), 
                          strBorderColor.c_str(), 
                          strSampleCount.c_str(),
                          width, height, depth,
                          animChunkX, animChunkY, 
                          isAlpha ? "true" : "false",
                          isRT ? "true" : "false", 
                          isGCS ? "true" : "false");
            }
        }

        return true;
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

}; //LostPeterEngine