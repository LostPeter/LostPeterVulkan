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


    //Set TextureParam
    void TextureManager::SetTextureParam_Width(NameValuePairMap& mapParam, uint32 nWidth)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_Width), FUtilString::SaveUInt(nWidth));
    }
    void TextureManager::SetTextureParam_Height(NameValuePairMap& mapParam, uint32 nHeight)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_Height), FUtilString::SaveUInt(nHeight));
    }
    void TextureManager::SetTextureParam_Depth(NameValuePairMap& mapParam, uint32 nDepth)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_Depth), FUtilString::SaveUInt(nDepth));
    }

    //Get TextureParam
    uint32 TextureManager::GetTextureParam_Width(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Width);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            F_LogError("*********************** TextureManager::GetTextureParam_Width: Can not find param name: [%s] from param map !", strName.c_str());
            return 0;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_Height(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Height);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            F_LogError("*********************** TextureManager::GetTextureParam_Height: Can not find param name: [%s] from param map !", strName.c_str());
            return 0;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_Depth(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Depth);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            F_LogError("*********************** TextureManager::GetTextureParam_Depth: Can not find param name: [%s] from param map !", strName.c_str());
            return 0;
        }
        return FUtilString::ParserUInt(itFind->second);
    }


    TextureManager::TextureManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Texture), E_Resource_Texture)
        , m_pTextureSerializer(nullptr)
        , m_nBitDepthIntegerPreferred(0)
		, m_nBitDepthFloatPreferred(0)
		, m_nMipMapsCountDefault(E_TextureMipMap_UnLimited)
		, m_fMipMapLODBiasDefault(0.0f)
    {
        m_fLoadingOrder = 75.0f;
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
                    pTexture->Load();
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
                    pTexture->Load();
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
                    pTexture->Load();
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


    ResourceCreateOrRetrieveResult TextureManager::CreateOrRetrieve(uint32 nGroup, 
                                                                    const String& strName, 
                                                                    const String& strGroupName, 
                                                                    bool bIsManualLoad /*= false*/,
                                                                    ResourceManualLoader* pManualLoader /*= nullptr*/, 
                                                                    const NameValuePairMap* pLoadParams /*= nullptr*/,
                                                                    FTextureType eTexture /*= F_Texture_2D*/, 
                                                                    int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                                                    float fGamma /*= 1.0f*/,
                                                                    bool bIsAlpha /*= false*/,
                                                                    FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                                                    bool bIsHWGammaCorrection /*= false*/)
    {
        NameValuePairMap mapTextureParam;
        if (bIsManualLoad && pLoadParams)
        {
            FUtil::CopyNameValuePairMapTo(pLoadParams, &mapTextureParam);
        }


        ResourceCreateOrRetrieveResult result = ResourceManager::CreateOrRetrieve(nGroup,
                                                                                  strName,
                                                                                  strGroupName,
                                                                                  bIsManualLoad,
                                                                                  pManualLoader,
                                                                                  &mapTextureParam);
		if (!result.second || !result.second)
		{
            F_LogError("*********************** TextureManager::CreateOrRetrieve: CreateOrRetrieve texture failed, group: [%d], name: [%s]", nGroup, strName.c_str());
			//Texture* pTexture = (Texture*)result.first;
			// pTexture->SetTextureType(eTexture);
			// pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
			// pTexture->SetGamma(fGamma);
			// pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
			// pTexture->SetPixelFormat(ePixelFormat);
			// pTexture->SetHardwareGammaEnabled(bIsHWGammaCorrection);
		}
		return result;
    }


    Texture* TextureManager::Prepare(uint32 nGroup, 
                                     const String& strName, 
                                     const String& strGroupName, 
                                     FTextureType eTexture /*= F_Texture_2D*/, 
                                     int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                     bool fGamma /*= 1.0f*/, 
                                     bool bIsAlpha /*= false*/,
                                     FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                     bool bIsHWGammaCorrection /*= false*/)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieve(nGroup,
                                                                 strName,
                                                                 strGroupName,
                                                                 false,
                                                                 nullptr,
                                                                 nullptr,
                                                                 eTexture,
                                                                 nMipMapsCount,
                                                                 fGamma,
                                                                 bIsAlpha,
                                                                 ePixelFormat,
                                                                 bIsHWGammaCorrection);
		Texture* pTexture = (Texture*)result.first;
        if (!pTexture)
            return nullptr;
		pTexture->Prepare();
		return pTexture;
    }


    Texture* TextureManager::Load(uint32 nGroup, 
                                  const String& strName, 
                                  const String& strGroupName, 
                                  FTextureType eTexture /*= F_Texture_2D*/, 
                                  int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                  bool fGamma /*= 1.0f*/,
                                  bool bIsAlpha /*= false*/,
                                  FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                  bool bIsHWGammaCorrection /*= false*/)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieve(nGroup,
                                                                 strName,
                                                                 strGroupName,
                                                                 false,
                                                                 nullptr,
                                                                 nullptr,
                                                                 eTexture,
                                                                 nMipMapsCount,
                                                                 fGamma,
                                                                 bIsAlpha,
                                                                 ePixelFormat,
                                                                 bIsHWGammaCorrection);
		Texture* pTexture = (Texture*)result.first;
        if (!pTexture)
            return nullptr;
		pTexture->Load();
		return pTexture;
    }


    Texture* TextureManager::CreateTexture(uint32 nGroup, 
                                           const String& strName, 
                                           const String& strGroupName, 
                                           FTextureType eTexture /*= F_Texture_2D*/, 
                                           int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
				                           float fGamma /*= 1.0f*/, 
                                           bool bIsAlpha /*= false*/, 
                                           FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                           bool bUseMemoryImage /*= false*/,
				                           bool bBackground /*= true*/, 
                                           uint16 nRecoveryGroupID /*= 0*/)
	{
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}

        NameValuePairMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		pTexture = (Texture*)createImpl(nGroup, 
                                        strName, 
                                        strGroupName,
                                        getNextHandle(),
                                        false,
                                        nullptr,
                                        &mapParams);
		pTexture->AddRef();
		
        pTexture->SetTextureType(eTexture);
        pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
        pTexture->SetGamma(fGamma);
        pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
        pTexture->SetPixelFormat(ePixelFormat);
        pTexture->Load();

        AddTexture(nGroup, pTexture);
        return pTexture;
	}

	Texture* TextureManager::CreateTextureManual(uint32 nGroup, 
                                                 const String& strName, 
                                                 const String& strGroupName, 
                                                 ResourceManualLoader* pManualLoader,
                                                 FTextureType eTexture, 
                                                 uint32 nWidth, 
                                                 uint32 nHeight, 
                                                 uint32 nDepth, 
		                                         int32 nMipMapsCount, 
                                                 FPixelFormatType ePixelFormat, 
                                                 uint32 nUsage /*= E_TextureUsage_Default*/, 
                                                 bool bUseMemoryImage /*= false*/, 
                                                 uint32 nFSAA /*= 0*/)
	{
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}

        NameValuePairMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		pTexture = (Texture*)createImpl(nGroup, 
                                        strName, 
                                        strGroupName,
                                        getNextHandle(),
                                        true,
                                        pManualLoader,
                                        &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetWidth(nWidth);
		pTexture->SetHeight(nHeight);
		pTexture->SetDepth(nDepth);
		pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
		pTexture->SetPixelFormat(ePixelFormat);
		pTexture->SetUsage(nUsage);
		pTexture->SetFSAA(nFSAA);
		pTexture->CreateInternalResources();

        AddTexture(nGroup, pTexture);
		return pTexture;
	}

	Texture* TextureManager::CreateTextureFromImage(uint32 nGroup, 
                                                    const String& strName, 
                                                    const String& strGroupName, 
                                                    Image* pImage, 
                                                    FTextureType eTexture /*= F_Texture_2D*/,
				                                    int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                                    float fGamma /*= 1.0f*/, 
                                                    bool bIsAlpha /*= false*/,
				                                    FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                                    bool bUseMemoryImage /*= false*/)
	{
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}

        NameValuePairMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		pTexture = (Texture*)createImpl(nGroup, 
                                        strName, 
                                        strGroupName,
                                        getNextHandle(),
                                        false,
                                        nullptr,
                                        &mapParams);
		pTexture->AddRef();
		
		pTexture->SetTextureType(eTexture);
		pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
		pTexture->SetGamma(fGamma);
		pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
		pTexture->SetPixelFormat(ePixelFormat);
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
                                                       const String& strGroupName, 
                                                       FFileMemory* pInput, 
                                                       FTextureType eTexture /*= F_Texture_2D*/,
                                                       int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                                       float fGamma /*= 1.0f*/, 
                                                       bool bIsAlpha /*= false*/,
                                                       FPixelFormatType ePixelFormat /*= F_PixelFormat_Unknown*/, 
                                                       bool bUseMemoryImage /*= false*/)
	{
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}

        NameValuePairMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		pTexture = (Texture*)createImpl(nGroup, 
                                        strName, 
                                        strGroupName,
                                        getNextHandle(),
                                        false,
                                        nullptr,
                                        &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
		pTexture->SetGamma(fGamma);
		pTexture->SetTreatLuminanceAsAlpha(bIsAlpha);
		pTexture->SetPixelFormat(ePixelFormat);
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
                                                      const String& strGroupName, 
                                                      FFileMemory* pInput, 
                                                      uint32 nWidth, 
                                                      uint32 nHeight, 
				                                      FPixelFormatType ePixelFormat, 
                                                      FTextureType eTexture /*= F_Texture_2D*/, 
                                                      int32 nMipMapsCount /*= E_TextureMipMap_Default*/, 
                                                      float fGamma /*= 1.0f*/, 
                                                      bool bUseMemoryImage /*= false*/)
	{
		Texture* pTexture = GetTexture(nGroup, strName);
		if (pTexture)
		{
			pTexture->AddRef();
			return pTexture;
		}

        NameValuePairMap mapParams;
		mapParams["UseMemoryImage"] = FUtilString::SaveBool(bUseMemoryImage);
		pTexture = (Texture*)createImpl(nGroup,
                                        strName, 
                                        strGroupName,
                                        getNextHandle(),
                                        false,
                                        nullptr,
                                        &mapParams);
		pTexture->AddRef();

		pTexture->SetTextureType(eTexture);
		pTexture->SetMipMapsCount((nMipMapsCount == E_TextureMipMap_Default) ? m_nMipMapsCountDefault : (uint32)nMipMapsCount);
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