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
#include "../include/Image.h"

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
        , m_nBitDepthIntegerPreferred(0)
		, m_nBitDepthFloatPreferred(0)
		, m_nNumMipMapsDefault(E_TextureMipMap_UnLimited)
		, m_fMipMapLODBiasDefault(0.0f)
    {

    }
    TextureManager::~TextureManager()
    {
        
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
        Texture* pTexture = nullptr;

        // if (AddTexture(pTI->group, pTexture))
        // {
        //     F_LogInfo("TextureManager::loadTexture: Load texture success, [%u]-[%s]-[%s] !",
        //               pTI->group, 
        //               pTI->nameTexture.c_str(), 
        //               pTI->pathTexture.c_str());
        // }
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

    void TextureManager::SetBitDepthIntegerPreferred(uint16 nBitDepthIntegerPreferred, bool bReloadTextures /*= true*/)
	{
		m_nBitDepthIntegerPreferred = nBitDepthIntegerPreferred;

		if (bReloadTextures)
		{		
			for (TexturePtrVector::iterator it = m_aTexture.begin(); 
				 it != m_aTexture.end(); ++it)
			{
				Texture* pTexture = (*it);
				if (pTexture->IsLoaded() && !pTexture->IsManual())
				{
					pTexture->Unload();
					pTexture->SetBitDepthIntegerDesired(nBitDepthIntegerPreferred);
                    if (!pTexture->Load())
                    {
                        F_LogError("*********************** TextureManager::SetBitDepthIntegerPreferred: Load texture failed, group: [%d], name: [%s]", pTexture->GetGroup(), pTexture->GetName().c_str());
                        continue;
                    }
				}
				else
				{
					pTexture->SetBitDepthIntegerDesired(nBitDepthIntegerPreferred);
				}
			}	
		}
	}

	void TextureManager::SetBitDepthFloatPreferred(uint16 nBitDepthFloatPreferred, bool bReloadTextures /*= true*/)
	{
		m_nBitDepthFloatPreferred = nBitDepthFloatPreferred;

		if (bReloadTextures)
		{		
			for (TexturePtrVector::iterator it = m_aTexture.begin(); 
				 it != m_aTexture.end(); ++it)
			{
				Texture* pTexture = (*it);
				if (pTexture->IsLoaded() && !pTexture->IsManual())
				{
					pTexture->Unload();
					pTexture->SetBitDepthFloatDesired(nBitDepthFloatPreferred);
                    if (!pTexture->Load())
                    {
                        F_LogError("*********************** TextureManager::SetBitDepthFloatPreferred: Load texture failed, group: [%d], name: [%s]", pTexture->GetGroup(), pTexture->GetName().c_str());
                        continue;
                    }
				}
				else
				{
					pTexture->SetBitDepthFloatDesired(nBitDepthFloatPreferred);
				}
			}           
		}
	}

	void TextureManager::SetBitDepthsPreferred(uint16 nBitDepthIntegerPreferred, uint16 nBitDepthFloatPreferred, bool bReloadTextures /*= true*/)
	{
		m_nBitDepthIntegerPreferred = nBitDepthIntegerPreferred;
		m_nBitDepthFloatPreferred = nBitDepthFloatPreferred;

		if (bReloadTextures)
		{
			for (TexturePtrVector::iterator it = m_aTexture.begin(); 
				 it != m_aTexture.end(); ++it)
			{
				Texture* pTexture = (*it);
				if (pTexture->IsLoaded() && !pTexture->IsManual())
				{
					pTexture->Unload();
					pTexture->SetBitDepthsDesired(nBitDepthIntegerPreferred, nBitDepthFloatPreferred);
                    if (!pTexture->Load())
                    {
                        F_LogError("*********************** TextureManager::SetBitDepthsPreferred: Load texture failed, group: [%d], name: [%s]", pTexture->GetGroup(), pTexture->GetName().c_str());
                        continue;
                    }
				}
				else
				{
					pTexture->SetBitDepthsDesired(nBitDepthIntegerPreferred, nBitDepthFloatPreferred);
				}
			}			
		}
	}

	bool TextureManager::IsPixelFormatSupported(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage)
	{
		return GetPixelFormatNative(eTexture, ePixelFormat, nUsage) == ePixelFormat;
	}

	bool TextureManager::IsPixelFormatSupportedEquivalent(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage)
	{	
		FPixelFormatType eSupportedPF = GetPixelFormatNative(eTexture, ePixelFormat, nUsage);
		return FPixelFormat::GetPixelFormatElemBits(eSupportedPF) >= FPixelFormat::GetPixelFormatElemBits(ePixelFormat);
	}

    Texture* TextureManager::CreateTexture(uint32 nGroup, 
                                           const String& strName, 
                                           FTextureType eTexture /*= F_Texture_2D*/, 
                                           int nNumMipMaps /*= E_TextureMipMap_Default*/, 
				                           float fGamma /*= 1.0f*/, 
                                           bool bIsAlpha /*= false*/, 
                                           FPixelFormatType ePixelFormatDesired /*= F_PixelFormat_Unknown*/, 
                                           bool bUseMemoryImage /*= false*/,
				                           bool bBackground /*= true*/, 
                                           uint16 nRecoveryGroupID /*= 0*/)
	{
		String2StringMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}
		pTexture = createImpl(nGroup, strName, &mapParams);
		pTexture->AddRef();
		
        pTexture->SetTextureType(eTexture);
        pTexture->SetNumMipMaps((nNumMipMaps == E_TextureMipMap_Default) ? m_nNumMipMapsDefault : static_cast<size_t>(nNumMipMaps));
        pTexture->SetGamma(fGamma);
        pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
        pTexture->SetPixelFormat(ePixelFormatDesired);
        if (!pTexture->Load())
        {
            F_LogError("*********************** TextureManager::CreateTexture: Load texture failed, group: [%d], name: [%s]", nGroup, strName.c_str());
            F_DELETE(pTexture)
            return nullptr;
        }

        AddTexture(nGroup, pTexture);
        return pTexture;
	}

	Texture* TextureManager::CreateTextureManual(uint32 nGroup, 
                                                 const String& strName, 
                                                 FTextureType eTexture, 
                                                 uint32 nWidth, 
                                                 uint32 nHeight, 
                                                 uint32 nDepth, 
		                                         int nNumMipMaps, 
                                                 FPixelFormatType ePixelFormat, 
                                                 uint32 nUsage /*= E_TextureUsage_Default*/, 
                                                 bool bUseMemoryImage /*= false*/, 
                                                 uint32 nFSAA /*= 0*/)
	{
		String2StringMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}
		pTexture = createImpl(nGroup, strName, &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetWidth(nWidth);
		pTexture->SetHeight(nHeight);
		pTexture->SetDepth(nDepth);
		pTexture->SetNumMipMaps((nNumMipMaps == E_TextureMipMap_Default) ? m_nNumMipMapsDefault : static_cast<size_t>(nNumMipMaps));
		pTexture->SetPixelFormat(ePixelFormat);
		pTexture->SetUsage(nUsage);
		pTexture->SetFSAA(nFSAA);
		pTexture->CreateInternalResources();

        AddTexture(nGroup, pTexture);
		return pTexture;
	}

	Texture* TextureManager::CreateTextureFromImage(uint32 nGroup, 
                                                    const String& strName, 
                                                    Image* pImage, 
                                                    FTextureType eTexture /*= F_Texture_2D*/,
				                                    int nNumMipMaps /*= E_TextureMipMap_Default*/, 
                                                    float fGamma /*= 1.0f*/, 
                                                    bool bIsAlpha /*= false*/,
				                                    FPixelFormatType ePixelFormatDesired /*= F_PixelFormat_Unknown*/, 
                                                    bool bUseMemoryImage /*= false*/)
	{
		String2StringMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}
		pTexture = createImpl(nGroup, strName, &mapParams);
		pTexture->AddRef();
		
		pTexture->SetTextureType(eTexture);
		pTexture->SetNumMipMaps((nNumMipMaps == E_TextureMipMap_Default) ? m_nNumMipMapsDefault : static_cast<size_t>(nNumMipMaps));
		pTexture->SetGamma(fGamma);
		pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
		pTexture->SetPixelFormat(ePixelFormatDesired);
		if (!pTexture->LoadFromImage(pImage))
        {
            F_LogError("*********************** TextureManager::CreateTextureFromImage: Load texture from image failed, group: [%d], name: [%s]", nGroup, strName.c_str());
            F_DELETE(pTexture)
            return nullptr;
        }

        AddTexture(nGroup, pTexture);
		return pTexture;
	}

	Texture* TextureManager::CreateTextureFromDDSImage(uint32 nGroup, 
                                                       const String& strName, 
                                                       FFileMemory* pInput, 
                                                       FTextureType eTexture /*= F_Texture_2D*/,
                                                       int nNumMipMaps /*= E_TextureMipMap_Default*/, 
                                                       float fGamma /*= 1.0f*/, 
                                                       bool bIsAlpha /*= false*/,
                                                       FPixelFormatType ePixelFormatDesired /*= F_PixelFormat_Unknown*/, 
                                                       bool bUseMemoryImage /*= false*/)
	{
		String2StringMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}
		pTexture = createImpl(nGroup, strName, &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetNumMipMaps((nNumMipMaps == E_TextureMipMap_Default) ? m_nNumMipMapsDefault : static_cast<size_t>(nNumMipMaps));
		pTexture->SetGamma(fGamma);
		pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
		pTexture->SetPixelFormat(ePixelFormatDesired);
		if (!pTexture->LoadFromDDSImage(pInput))
        {
            F_LogError("*********************** TextureManager::CreateTextureFromDDSImage: Load texture from dds image failed, group: [%d], name: [%s]", nGroup, strName.c_str());
            F_DELETE(pTexture)
            return nullptr;
        }

        AddTexture(nGroup, pTexture);
		return pTexture;
	}

	Texture* TextureManager::CreateTextureFromRawData(uint32 nGroup, 
                                                      const String& strName, 
                                                      FFileMemory* pInput, 
                                                      size_t nWidth, 
                                                      size_t nHeight, 
				                                      FPixelFormatType ePixelFormat, 
                                                      FTextureType eTexture /*= F_Texture_2D*/, 
                                                      int nNumMipMaps /*= E_TextureMipMap_Default*/, 
                                                      float fGamma /*= 1.0f*/, 
                                                      bool bUseMemoryImage /*= false*/)
	{
		String2StringMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}
		pTexture = createImpl(nGroup, strName, &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetNumMipMaps((nNumMipMaps == E_TextureMipMap_Default) ? m_nNumMipMapsDefault : static_cast<size_t>(nNumMipMaps));
		pTexture->SetGamma(fGamma);
		if (!pTexture->LoadFromRawData(pInput, nWidth, nHeight, ePixelFormat))
        {
            F_LogError("*********************** TextureManager::CreateTextureFromRawData: Load texture from raw data failed, group: [%d], name: [%s]", nGroup, strName.c_str());
            F_DELETE(pTexture)
            return nullptr;
        }

        AddTexture(nGroup, pTexture);
		return pTexture;
	}

}; //LostPeterEngine