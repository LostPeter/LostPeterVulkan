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
    void TextureManager::SetTextureParam_Usage(NameValuePairMap& mapParam, uint32 nUsage)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_Usage), FUtilString::SaveUInt(nUsage));
    }
    void TextureManager::SetTextureParam_TextureType(NameValuePairMap& mapParam, FTextureType eTexture)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_TextureType), F_GetTextureTypeName(eTexture));
    }
    void TextureManager::SetTextureParam_TextureFilterType(NameValuePairMap& mapParam, FTextureFilterType eTextureFilter)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_TextureFilterType), F_GetTextureFilterTypeName(eTextureFilter));
    }
    void TextureManager::SetTextureParam_TextureAddressingType(NameValuePairMap& mapParam, FTextureAddressingType eTextureAddressing)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_TextureAddressingType), F_GetTextureAddressingTypeName(eTextureAddressing));
    }
    void TextureManager::SetTextureParam_TextureBorderColorType(NameValuePairMap& mapParam, FTextureBorderColorType eTextureBorderColor)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_TextureBorderColorType), F_GetTextureBorderColorTypeName(eTextureBorderColor));
    }
    void TextureManager::SetTextureParam_MSAASampleCount(NameValuePairMap& mapParam, FMSAASampleCountType eMSAASampleCount)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_MSAASampleCount), F_GetMSAASampleCountTypeName(eMSAASampleCount));
    }
    void TextureManager::SetTextureParam_PixelFormatType(NameValuePairMap& mapParam, FPixelFormatType ePixelFormat)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_PixelFormatType), FPixelFormat::GetPixelFormatDes(ePixelFormat).name);
    }
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
    void TextureManager::SetTextureParam_BitDepthInteger(NameValuePairMap& mapParam, uint16 nBitDepthInteger)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_BitDepthInteger), FUtilString::SaveUInt16(nBitDepthInteger));
    }
    void TextureManager::SetTextureParam_BitDepthFloat(NameValuePairMap& mapParam, uint16 nBitDepthFloat)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_BitDepthFloat), FUtilString::SaveUInt16(nBitDepthFloat));
    }
    void TextureManager::SetTextureParam_IsTreatLuminanceAsAlpha(NameValuePairMap& mapParam, bool bIsTreatLuminanceAsAlpha)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_IsTreatLuminanceAsAlpha), FUtilString::SaveBool(bIsTreatLuminanceAsAlpha));
    }
    void TextureManager::SetTextureParam_MipMapsCount(NameValuePairMap& mapParam, uint32 nMipMapsCount)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_MipMapsCount), FUtilString::SaveUInt(nMipMapsCount));
    }
    void TextureManager::SetTextureParam_IsMipMapsHardwareGenerated(NameValuePairMap& mapParam, bool bIsMipMapsHardwareGenerated)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_IsMipMapsHardwareGenerated), FUtilString::SaveBool(bIsMipMapsHardwareGenerated));
    }
    void TextureManager::SetTextureParam_Gamma(NameValuePairMap& mapParam, float fGamma)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_Gamma), FUtilString::SaveFloat(fGamma));
    }
    void TextureManager::SetTextureParam_IsGammaHardware(NameValuePairMap& mapParam, bool bIsGammaHardware)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetTextureParamTypeName(E_TextureParam_IsGammaHardware), FUtilString::SaveBool(bIsGammaHardware));
    }
    

    //Get TextureParam
    uint32 TextureManager::GetTextureParam_Usage(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Usage);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_Usage: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nUsage_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    FTextureType TextureManager::GetTextureParam_TextureType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_TextureType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_TextureType: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_eTexture_Default;
        }
        return F_ParseTextureType(itFind->second);
    }
    FTextureFilterType TextureManager::GetTextureParam_TextureFilterType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_TextureFilterType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_TextureFilterType: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_eTextureFilter_Default;
        }
        return F_ParseTextureFilterType(itFind->second);
    }
    FTextureAddressingType TextureManager::GetTextureParam_TextureAddressingType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_TextureAddressingType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_TextureAddressingType: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_eTextureAddressing_Default;
        }
        return F_ParseTextureAddressingType(itFind->second);
    }
    FTextureBorderColorType TextureManager::GetTextureParam_TextureBorderColorType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_TextureBorderColorType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_TextureBorderColorType: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_eTextureBorderColor_Default;
        }
        return F_ParseTextureBorderColorType(itFind->second);
    }
    FPixelFormatType TextureManager::GetTextureParam_PixelFormatType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_TextureBorderColorType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_PixelFormatType: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_ePixelFormat_Default;
        }
        return FPixelFormat::ParsePixelFormatFromName(itFind->second);
    }
    FMSAASampleCountType TextureManager::GetTextureParam_MSAASampleCount(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_MSAASampleCount);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::E_GetTextureParamTypeName: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_eMSAASampleCount_Default;
        }
        return F_ParseMSAASampleCountType(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_Width(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Width);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_Width: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nWidth_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_Height(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Height);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_Height: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nHeight_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_Depth(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Depth);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_Depth: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nDepth_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }   
    uint16 TextureManager::GetTextureParam_BitDepthInteger(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_BitDepthInteger);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_BitDepthInteger: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nBitDepthInteger_Default;
        }
        return FUtilString::ParserUInt16(itFind->second);
    }
    uint16 TextureManager::GetTextureParam_BitDepthFloat(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_BitDepthFloat);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_BitDepthFloat: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nBitDepthFloat_Default;
        }
        return FUtilString::ParserUInt16(itFind->second);
    }
    bool TextureManager::GetTextureParam_IsTreatLuminanceAsAlpha(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_IsTreatLuminanceAsAlpha);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_IsTreatLuminanceAsAlpha: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_bIsTreatLuminanceAsAlpha_Default;
        }
        return FUtilString::ParserBool(itFind->second);
    }
    uint32 TextureManager::GetTextureParam_MipMapsCount(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_MipMapsCount);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_MipMapsCount: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_nMipMapsCount_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    bool TextureManager::GetTextureParam_IsMipMapsHardwareGenerated(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_IsMipMapsHardwareGenerated);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_IsMipMapsHardwareGenerated: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_bIsMipMapsHardwareGenerated_Default;
        }
        return FUtilString::ParserBool(itFind->second);
    }
    float TextureManager::GetTextureParam_Gamma(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_Gamma);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_Gamma: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_fGamma_Default;
        }
        return FUtilString::ParserFloat(itFind->second);
    }
    bool TextureManager::GetTextureParam_IsGammaHardware(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetTextureParamTypeName(E_TextureParam_IsGammaHardware);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** TextureManager::GetTextureParam_IsGammaHardware: Can not find param name: [%s] from param map !", strName.c_str());
            return TextureManager::ms_bIsGammaHardware_Default;
        }
        return FUtilString::ParserBool(itFind->second);
    }


    NameValuePairMap TextureManager::ms_mapParam_Default;
        uint32 TextureManager::ms_nUsage_Default = E_TextureUsage_Default;
        FTextureType TextureManager::ms_eTexture_Default = F_Texture_2D;
        FTextureFilterType TextureManager::ms_eTextureFilter_Default = F_TextureFilter_Bilinear;
        FTextureAddressingType TextureManager::ms_eTextureAddressing_Default = F_TextureAddressing_Clamp;
        FTextureBorderColorType TextureManager::ms_eTextureBorderColor_Default = F_TextureBorderColor_OpaqueBlack;
        FMSAASampleCountType TextureManager::ms_eMSAASampleCount_Default = F_MSAASampleCount_1_Bit;
        FPixelFormatType TextureManager::ms_ePixelFormat_Default = F_PixelFormat_BYTE_A8R8G8B8_SRGB;
        uint32 TextureManager::ms_nWidth_Default = 512;
        uint32 TextureManager::ms_nHeight_Default = 512;
        uint32 TextureManager::ms_nDepth_Default = 1;
        uint16 TextureManager::ms_nBitDepthInteger_Default = 32;
        uint16 TextureManager::ms_nBitDepthFloat_Default = 32;
        bool TextureManager::ms_bIsTreatLuminanceAsAlpha_Default = false;
        uint32 TextureManager::ms_nMipMapsCount_Default = 0;
        bool TextureManager::ms_bIsMipMapsHardwareGenerated_Default = true;
        float TextureManager::ms_fGamma_Default = 1.0f;
        bool TextureManager::ms_bIsGammaHardware_Default = false;

    
    TextureManager::TextureManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Texture), E_Resource_Texture)
        , m_pTextureSerializer(nullptr)
        , m_nBitDepthIntegerPreferred(32)
		, m_nBitDepthFloatPreferred(32)
		, m_nMipMapsCountDefault(E_TextureMipMap_UnLimited)
		, m_fMipMapLODBiasDefault(0.0f)
    {
        m_fLoadingOrder = 75.0f;

        if (ms_mapParam_Default.size() <= 0)
        {
            SetTextureParam_Usage(ms_mapParam_Default, TextureManager::ms_nUsage_Default);
            SetTextureParam_TextureType(ms_mapParam_Default, TextureManager::ms_eTexture_Default);
            SetTextureParam_TextureFilterType(ms_mapParam_Default, TextureManager::ms_eTextureFilter_Default);
            SetTextureParam_TextureAddressingType(ms_mapParam_Default, TextureManager::ms_eTextureAddressing_Default);
            SetTextureParam_TextureBorderColorType(ms_mapParam_Default, TextureManager::ms_eTextureBorderColor_Default);
            SetTextureParam_MSAASampleCount(ms_mapParam_Default, TextureManager::ms_eMSAASampleCount_Default);
            SetTextureParam_PixelFormatType(ms_mapParam_Default,TextureManager::ms_ePixelFormat_Default);
            SetTextureParam_Width(ms_mapParam_Default, TextureManager::ms_nWidth_Default);
            SetTextureParam_Height(ms_mapParam_Default, TextureManager::ms_nHeight_Default);
            SetTextureParam_Depth(ms_mapParam_Default, TextureManager::ms_nDepth_Default);
            SetTextureParam_BitDepthInteger(ms_mapParam_Default, TextureManager::ms_nBitDepthInteger_Default);
            SetTextureParam_BitDepthFloat(ms_mapParam_Default, TextureManager::ms_nBitDepthFloat_Default);
            SetTextureParam_IsTreatLuminanceAsAlpha(ms_mapParam_Default, TextureManager::ms_bIsTreatLuminanceAsAlpha_Default);
            SetTextureParam_MipMapsCount(ms_mapParam_Default, TextureManager::ms_nMipMapsCount_Default);
            SetTextureParam_IsMipMapsHardwareGenerated(ms_mapParam_Default, TextureManager::ms_bIsMipMapsHardwareGenerated_Default);
            SetTextureParam_Gamma(ms_mapParam_Default, TextureManager::ms_fGamma_Default);
            SetTextureParam_IsGammaHardware(ms_mapParam_Default, TextureManager::ms_bIsGammaHardware_Default);
        }
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
				if (pTexture->IsLoaded() && !pTexture->IsManualLoad())
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
				if (pTexture->IsLoaded() && !pTexture->IsManualLoad())
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
				if (pTexture->IsLoaded() && !pTexture->IsManualLoad())
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
                                                                    uint32 nUsage /*= TextureManager::ms_nUsage_Default*/,
                                                                    FTextureType eTexture /*= TextureManager::ms_nUsage_Default*/, 
                                                                    FTextureFilterType eTextureFilter /*= TextureManager::ms_eTextureFilter_Default*/,
                                                                    FTextureAddressingType eTextureAddressing /*= TextureManager::ms_eTextureAddressing_Default*/,
                                                                    FTextureBorderColorType eTextureBorderColor /*= TextureManager::ms_eTextureBorderColor_Default*/,
                                                                    FMSAASampleCountType eMSAASampleCount /*= TextureManager::ms_eMSAASampleCount_Default*/,
                                                                    FPixelFormatType ePixelFormat /*= TextureManager::ms_ePixelFormat_Default*/, 
                                                                    uint32 nWidth /*= TextureManager::ms_nWidth_Default*/,
                                                                    uint32 nHeight /*= TextureManager::ms_nHeight_Default*/,
                                                                    uint32 nDepth /*= TextureManager::ms_nDepth_Default*/,
                                                                    uint16 nBitDepthInteger /*= TextureManager::ms_nBitDepthInteger_Default*/,
                                                                    uint16 nBitDepthFloat /*= TextureManager::ms_nBitDepthFloat_Default*/,
                                                                    bool bIsTreatLuminanceAsAlpha /*= TextureManager::ms_bIsTreatLuminanceAsAlpha_Default*/,
                                                                    int32 nMipMapsCount /*= TextureManager::ms_nMipMapsCount_Default*/, 
                                                                    bool bIsMipMapsHardwareGenerated /*= TextureManager::ms_bIsMipMapsHardwareGenerated_Default*/,
                                                                    float fGamma /*= TextureManager::ms_fGamma_Default*/,
                                                                    bool bIsGammaHardware /*= TextureManager::ms_bIsGammaHardware_Default*/)
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
			// pTexture->SetIsTreatLuminanceAsAlpha(bIsTreatLuminanceAsAlpha);
			// pTexture->SetPixelFormat(ePixelFormat);
			// pTexture->SetIsGammaHardware(bIsHWGammaCorrection);
		}
		return result;
    }


    Texture* TextureManager::Prepare(uint32 nGroup, 
                                     const String& strName, 
                                     const String& strGroupName, 
                                     uint32 nUsage /*= TextureManager::ms_nUsage_Default*/,
                                     FTextureType eTexture /*= TextureManager::ms_nUsage_Default*/, 
                                     FTextureFilterType eTextureFilter /*= TextureManager::ms_eTextureFilter_Default*/,
                                     FTextureAddressingType eTextureAddressing /*= TextureManager::ms_eTextureAddressing_Default*/,
                                     FTextureBorderColorType eTextureBorderColor /*= TextureManager::ms_eTextureBorderColor_Default*/,
                                     FMSAASampleCountType eMSAASampleCount /*= TextureManager::ms_eMSAASampleCount_Default*/,
                                     FPixelFormatType ePixelFormat /*= TextureManager::ms_ePixelFormat_Default*/, 
                                     uint32 nWidth /*= TextureManager::ms_nWidth_Default*/,
                                     uint32 nHeight /*= TextureManager::ms_nHeight_Default*/,
                                     uint32 nDepth /*= TextureManager::ms_nDepth_Default*/,
                                     uint16 nBitDepthInteger /*= TextureManager::ms_nBitDepthInteger_Default*/,
                                     uint16 nBitDepthFloat /*= TextureManager::ms_nBitDepthFloat_Default*/,
                                     bool bIsTreatLuminanceAsAlpha /*= TextureManager::ms_bIsTreatLuminanceAsAlpha_Default*/,
                                     int32 nMipMapsCount /*= TextureManager::ms_nMipMapsCount_Default*/, 
                                     bool bIsMipMapsHardwareGenerated /*= TextureManager::ms_bIsMipMapsHardwareGenerated_Default*/,
                                     float fGamma /*= TextureManager::ms_fGamma_Default*/,
                                     bool bIsGammaHardware /*= TextureManager::ms_bIsGammaHardware_Default*/)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieve(nGroup,
                                                                 strName,
                                                                 strGroupName,
                                                                 false,
                                                                 nullptr,
                                                                 nullptr,
                                                                 nUsage,
                                                                 eTexture,
                                                                 eTextureFilter,
                                                                 eTextureAddressing,
                                                                 eTextureBorderColor,
                                                                 eMSAASampleCount,
                                                                 ePixelFormat,
                                                                 nWidth,
                                                                 nHeight,
                                                                 nDepth,
                                                                 nBitDepthInteger,
                                                                 nBitDepthFloat,
                                                                 bIsTreatLuminanceAsAlpha,
                                                                 nMipMapsCount,
                                                                 bIsMipMapsHardwareGenerated,
                                                                 fGamma,
                                                                 bIsGammaHardware);
		Texture* pTexture = (Texture*)result.first;
        if (!pTexture)
            return nullptr;
		pTexture->Prepare();
		return pTexture;
    }


    Texture* TextureManager::Load(uint32 nGroup, 
                                  const String& strName, 
                                  const String& strGroupName, 
                                  uint32 nUsage /*= TextureManager::ms_nUsage_Default*/,
                                  FTextureType eTexture /*= TextureManager::ms_nUsage_Default*/, 
                                  FTextureFilterType eTextureFilter /*= TextureManager::ms_eTextureFilter_Default*/,
                                  FTextureAddressingType eTextureAddressing /*= TextureManager::ms_eTextureAddressing_Default*/,
                                  FTextureBorderColorType eTextureBorderColor /*= TextureManager::ms_eTextureBorderColor_Default*/,
                                  FMSAASampleCountType eMSAASampleCount /*= TextureManager::ms_eMSAASampleCount_Default*/,
                                  FPixelFormatType ePixelFormat /*= TextureManager::ms_ePixelFormat_Default*/, 
                                  uint32 nWidth /*= TextureManager::ms_nWidth_Default*/,
                                  uint32 nHeight /*= TextureManager::ms_nHeight_Default*/,
                                  uint32 nDepth /*= TextureManager::ms_nDepth_Default*/,
                                  uint16 nBitDepthInteger /*= TextureManager::ms_nBitDepthInteger_Default*/,
                                  uint16 nBitDepthFloat /*= TextureManager::ms_nBitDepthFloat_Default*/,
                                  bool bIsTreatLuminanceAsAlpha /*= TextureManager::ms_bIsTreatLuminanceAsAlpha_Default*/,
                                  int32 nMipMapsCount /*= TextureManager::ms_nMipMapsCount_Default*/, 
                                  bool bIsMipMapsHardwareGenerated /*= TextureManager::ms_bIsMipMapsHardwareGenerated_Default*/,
                                  float fGamma /*= TextureManager::ms_fGamma_Default*/,
                                  bool bIsGammaHardware /*= TextureManager::ms_bIsGammaHardware_Default*/)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieve(nGroup,
                                                                 strName,
                                                                 strGroupName,
                                                                 false,
                                                                 nullptr,
                                                                 nullptr,
                                                                 nUsage,
                                                                 eTexture,
                                                                 eTextureFilter,
                                                                 eTextureAddressing,
                                                                 eTextureBorderColor,
                                                                 eMSAASampleCount,
                                                                 ePixelFormat,
                                                                 nWidth,
                                                                 nHeight,
                                                                 nDepth,
                                                                 nBitDepthInteger,
                                                                 nBitDepthFloat,
                                                                 bIsTreatLuminanceAsAlpha,
                                                                 nMipMapsCount,
                                                                 bIsMipMapsHardwareGenerated,
                                                                 fGamma,
                                                                 bIsGammaHardware);
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
                                           bool bIsTreatLuminanceAsAlpha /*= false*/, 
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
        pTexture->SetIsTreatLuminanceAsAlpha(bIsTreatLuminanceAsAlpha);
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
                                                    bool bIsTreatLuminanceAsAlpha /*= false*/,
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
		pTexture->SetIsTreatLuminanceAsAlpha(bIsTreatLuminanceAsAlpha);
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
                                                       bool bIsTreatLuminanceAsAlpha /*= false*/,
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
		pTexture->SetIsTreatLuminanceAsAlpha(bIsTreatLuminanceAsAlpha);
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