/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TextureManager.h"
#include "../include/TextureSerializer.h"
#include "../include/Texture.h"

template<> LostPeterEngine::TextureManager* LostPeterFoundation::FSingleton<LostPeterEngine::TextureManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    TextureManager* TextureManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	TextureManager& TextureManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "TextureManager::GetSingleton")
		return (*ms_Singleton);  
	}

    TextureManager::TextureManager()
        : Base("TextureManager")
        , m_pTextureSerializer(nullptr)
    {

    }
    TextureManager::~TextureManager()
    {
        Destroy();
    }

    void TextureManager::Destroy()
    {
        F_DELETE(m_pTextureSerializer)
        DeleteTextureAll();
    }
    bool TextureManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> Texture Cfg Path 
        String strPathCfgTexture = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgTexture.empty())
        {
            F_LogError("*********************** TextureManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Texture Serializer
        m_pTextureSerializer = new TextureSerializer();
        if (!m_pTextureSerializer->LoadFile(strPathCfgTexture))
        {
            F_LogError("*********************** TextureManager::Init: Load file texture cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        return true;
    }

    bool TextureManager::LoadTextureAll()
    {
        if (m_pTextureSerializer == nullptr)
            return false;

        DeleteTextureAll();
        TextureInfoPtrVector& aTextureInfos = m_pTextureSerializer->GetTextureInfoPtrVector();
        for (TextureInfoPtrVector::iterator it = aTextureInfos.begin();
             it != aTextureInfos.end(); ++it)
        {
            if (!loadTexture(*it))
                continue;
        }

        return true;
    }

    Texture* TextureManager::LoadTexture(uint nGroup, const String& strName)
    {
        if (m_pTextureSerializer == nullptr)
            return nullptr;

        Texture* pTexture = GetTexture(nGroup, strName);
        if (pTexture == nullptr)
        {
            TextureInfo* pTextureInfo = m_pTextureSerializer->GetTextureInfo(nGroup, strName);
            if (pTextureInfo == nullptr)
            {
                F_LogError("*********************** TextureManager::LoadTexture: Can not find texture info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadTexture(pTextureInfo))
            {
                return nullptr;
            }
        }
        pTexture->AddRef();
        return pTexture;
    }
    Texture* TextureManager::loadTexture(TextureInfo* pTI)
    {
        StringVector aPathTexture = FUtilString::Split(pTI->pathTexture, ";");
        Texture* pTexture = new Texture(pTI->group,
                                        pTI->nameTexture,
                                        aPathTexture,
                                        pTI->typeTexture,
                                        pTI->typeTexturePixelFormat,
                                        pTI->typeTextureFilter,
                                        pTI->typeTextureAddressing,
                                        pTI->typeTextureBorderColor,
                                        pTI->isRT,
                                        pTI->isGCS);
        pTexture->texChunkMaxX = pTI->animChunkX;
        pTexture->texChunkMaxY = pTI->animChunkY;
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
        }
        pTexture->LoadTexture(pTI->width, 
                              pTI->height,
                              pTI->depth);

        if (AddTexture(pTI->group, pTexture))
        {
            F_LogInfo("TextureManager::loadTexture: Load texture success, [%u]-[%s]-[%s] !",
                      pTI->group, 
                      pTI->nameTexture.c_str(), 
                      pTI->pathTexture.c_str());
        }
        return pTexture;
    }
    void TextureManager::UnloadTexture(Texture* pTexture)
    {
        if (pTexture == nullptr)
            return;
        pTexture->DelRef();
        if (!HasRef())
        {
            DeleteTexture(pTexture->GetGroup(), pTexture->GetName());
        }
    }

    bool TextureManager::HasTexture(uint nGroup, const String& strName)
    {
        return GetTexture(nGroup, strName) != nullptr;
    }

    Texture* TextureManager::GetTexture(uint nGroup, const String& strName)
    {
        TextureGroupPtrMap::iterator itFindGroup = m_mapTextureGroup.find(nGroup);
        if (itFindGroup == m_mapTextureGroup.end())
        {
            return nullptr;
        }

        TexturePtrMap::iterator itFindTexture = itFindGroup->second.find(strName);
        if (itFindTexture == itFindGroup->second.end())
        {
            return nullptr;
        }
        return itFindTexture->second;
    }

    bool TextureManager::AddTexture(uint nGroup, Texture* pTexture)
    {
        TextureGroupPtrMap::iterator itFind = m_mapTextureGroup.find(nGroup);
        if (itFind == m_mapTextureGroup.end())
        {
            TexturePtrMap mapTexture;
            m_mapTextureGroup[nGroup] = mapTexture;
            itFind = m_mapTextureGroup.find(nGroup);
        }
        const String& strName = pTexture->GetName();
        TexturePtrMap::iterator itFindTexture = itFind->second.find(strName);
        if (itFindTexture != itFind->second.end())
        {
            F_LogError("*********************** TextureManager::AddTexture: Texture name already exist: [%s] !", strName.c_str());
            F_DELETE(pTexture)
            return false;
        }

        itFind->second.insert(TexturePtrMap::value_type(strName, pTexture));
        m_aTexture.push_back(pTexture);
        return true;
    }

    void TextureManager::DeleteTexture(uint nGroup, const String& strName)
    {
        TextureGroupPtrMap::iterator itFind = m_mapTextureGroup.find(nGroup);
        if (itFind == m_mapTextureGroup.end())
        {
            return;
        }

        TexturePtrMap::iterator itFindTexture = itFind->second.find(strName);
        if (itFindTexture != itFind->second.end())
        {
            TexturePtrVector::iterator itFindA = std::find(m_aTexture.begin(), m_aTexture.end(), itFindTexture->second);
            if (itFindA != m_aTexture.end())
                m_aTexture.erase(itFindA);
            F_DELETE(itFindTexture->second)
            itFind->second.erase(itFindTexture);
        }
    }

    void TextureManager::DeleteTextureAll()
    {
        for (TextureGroupPtrMap::iterator it = m_mapTextureGroup.begin();
             it != m_mapTextureGroup.end(); ++it)
        {
            TexturePtrMap& mapTexture = it->second;
            for (TexturePtrMap::iterator itTexture = mapTexture.begin(); 
                 itTexture != mapTexture.end(); ++itTexture)
            {
                F_DELETE(itTexture->second)
            }
        }
        m_aTexture.clear();
        m_mapTextureGroup.clear();
    }

}; //LostPeterEngine