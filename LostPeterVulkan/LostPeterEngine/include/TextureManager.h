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

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "ResourceManager.h"

namespace LostPeterEngine
{
    class engineExport TextureManager : public FSingleton<TextureManager>
                                      , public ResourceManager
    {
    public:
        TextureManager();
        virtual ~TextureManager();

    public:
        static NameValuePairMap ms_mapParam_Default;
            static uint32 ms_nUsage_Default;
            static FTextureType ms_eTexture_Default;
            static FTextureFilterType ms_eTextureFilter_Default;
            static FTextureAddressingType ms_eTextureAddressing_Default;
            static FTextureBorderColorType ms_eTextureBorderColor_Default;
            static FMSAASampleCountType ms_eMSAASampleCount_Default;
            static FPixelFormatType ms_ePixelFormat_Default;
            static uint32 ms_nWidth_Default;
            static uint32 ms_nHeight_Default;
            static uint32 ms_nDepth_Default;
            static uint16 ms_nBitDepthInteger_Default;
            static uint16 ms_nBitDepthFloat_Default;
            static bool ms_bIsTreatLuminanceAsAlpha_Default;
            static uint32 ms_nMipMapsCount_Default;
            static bool ms_bIsMipMapsHardwareGenerated_Default;
            static float ms_fGamma_Default;
            static bool ms_bIsGammaHardware_Default;

        static const String& GetTextureParamValue(ETextureParamType type);

        //Set TextureParam
        static void SetTextureParam_Usage(NameValuePairMap& mapParam, uint32 nUsage);
        static void SetTextureParam_TextureType(NameValuePairMap& mapParam, FTextureType eTexture);
        static void SetTextureParam_TextureFilterType(NameValuePairMap& mapParam, FTextureFilterType eTextureFilter);
        static void SetTextureParam_TextureAddressingType(NameValuePairMap& mapParam, FTextureAddressingType eTextureAddressing);
        static void SetTextureParam_TextureBorderColorType(NameValuePairMap& mapParam, FTextureBorderColorType eTextureBorderColor);
        static void SetTextureParam_MSAASampleCount(NameValuePairMap& mapParam, FMSAASampleCountType eMSAASampleCount);
        static void SetTextureParam_PixelFormatType(NameValuePairMap& mapParam, FPixelFormatType ePixelFormat);
        static void SetTextureParam_Width(NameValuePairMap& mapParam, uint32 nWidth);
        static void SetTextureParam_Height(NameValuePairMap& mapParam, uint32 nHeight);
        static void SetTextureParam_Depth(NameValuePairMap& mapParam, uint32 nDepth);
        static void SetTextureParam_BitDepthInteger(NameValuePairMap& mapParam, uint16 nBitDepthInteger);
        static void SetTextureParam_BitDepthFloat(NameValuePairMap& mapParam, uint16 nBitDepthFloat);
        static void SetTextureParam_IsTreatLuminanceAsAlpha(NameValuePairMap& mapParam, bool bIsTreatLuminanceAsAlpha);
        static void SetTextureParam_MipMapsCount(NameValuePairMap& mapParam, uint32 nMipMapsCount);
        static void SetTextureParam_IsMipMapsHardwareGenerated(NameValuePairMap& mapParam, bool bIsMipMapsHardwareGenerated);
        static void SetTextureParam_Gamma(NameValuePairMap& mapParam, float fGamma);
        static void SetTextureParam_IsGammaHardware(NameValuePairMap& mapParam, bool bIsGammaHardware);

        //Get TextureParam
        static uint32 GetTextureParam_Usage(NameValuePairMap& mapParam);
        static FTextureType GetTextureParam_TextureType(NameValuePairMap& mapParam);
        static FTextureFilterType GetTextureParam_TextureFilterType(NameValuePairMap& mapParam);
        static FTextureAddressingType GetTextureParam_TextureAddressingType(NameValuePairMap& mapParam);
        static FTextureBorderColorType GetTextureParam_TextureBorderColorType(NameValuePairMap& mapParam);
        static FMSAASampleCountType GetTextureParam_MSAASampleCount(NameValuePairMap& mapParam);
        static FPixelFormatType GetTextureParam_PixelFormatType(NameValuePairMap& mapParam);
        static uint32 GetTextureParam_Width(NameValuePairMap& mapParam);
        static uint32 GetTextureParam_Height(NameValuePairMap& mapParam);
        static uint32 GetTextureParam_Depth(NameValuePairMap& mapParam);
        static uint16 GetTextureParam_BitDepthInteger(NameValuePairMap& mapParam);
        static uint16 GetTextureParam_BitDepthFloat(NameValuePairMap& mapParam);
        static bool GetTextureParam_IsTreatLuminanceAsAlpha(NameValuePairMap& mapParam);
        static uint32 GetTextureParam_MipMapsCount(NameValuePairMap& mapParam);
        static bool GetTextureParam_IsMipMapsHardwareGenerated(NameValuePairMap& mapParam);
        static float GetTextureParam_Gamma(NameValuePairMap& mapParam);
        static bool GetTextureParam_IsGammaHardware(NameValuePairMap& mapParam);

    public:
    protected:
        TextureSerializer* m_pTextureSerializer;
        TexturePtrVector m_aTexture;
        TextureGroupPtrMap m_mapTextureGroup;

        uint16 m_nBitDepthIntegerPreferred;
		uint16 m_nBitDepthFloatPreferred;
		uint32 m_nMipMapsCountDefault;
		float m_fMipMapLODBiasDefault;

    public:
        F_FORCEINLINE TextureSerializer* GetTextureSerializer() const { return m_pTextureSerializer; }
        F_FORCEINLINE const TexturePtrVector& GetTexturePtrVector() const { return m_aTexture; }
        F_FORCEINLINE TexturePtrVector& GetTexturePtrVector() { return m_aTexture; }
        F_FORCEINLINE const TextureGroupPtrMap& GetTextureGroupPtrMap() const { return m_mapTextureGroup; }
        F_FORCEINLINE TextureGroupPtrMap& GetTextureGroupPtrMap() { return m_mapTextureGroup; }

    public:
        static TextureManager& GetSingleton();
		static TextureManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        bool LoadTextureAll();
        Texture* LoadTexture(uint nGroup, const String& strName);
        void UnloadTexture(Texture* pTexture);
        
        bool HasTexture(uint nGroup, const String& strName);
        Texture* GetTexture(uint nGroup, const String& strName);
        bool AddTexture(uint nGroup, Texture* pTexture);
        void DeleteTexture(uint nGroup, const String& strName);
        void DeleteTextureAll();

    private:
        Texture* loadTexture(TextureInfo* pTI);

    public:
        virtual uint16 GetBitDepthIntegerPreferred() const { return m_nBitDepthIntegerPreferred; }
		virtual void SetBitDepthIntegerPreferred(uint16 nBitDepthIntegerPreferred, bool bReloadTextures = true);
		virtual uint16 GetBitDepthFloatPreferred() const { return m_nBitDepthFloatPreferred; }
		virtual void SetBitDepthFloatPreferred(uint16 nBitDepthFloatPreferred, bool bReloadTextures = true);
		virtual void SetBitDepthsPreferred(uint16 nBitDepthIntegerPreferred, uint16 nBitDepthFloatPreferred, bool bReloadTextures = true);
      
        virtual uint32 GetMipMapsCountDefault() { return m_nMipMapsCountDefault; }
		virtual void SetMipMapsCountDefault(uint32 nMipMapsCount) { m_nMipMapsCountDefault = nMipMapsCount; }
		virtual float GetMipMapLODBiasDefault() { return m_fMipMapLODBiasDefault; }
		virtual void SetMipMapLODBiasDefault(float fMipMapLODBiasDefault) { m_fMipMapLODBiasDefault = fMipMapLODBiasDefault; }
       
		virtual bool IsPixelFormatSupported(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage);
		virtual bool IsPixelFormatSupportedEquivalent(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage);

		virtual FPixelFormatType GetPixelFormatNative(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage) = 0;
		virtual bool IsHardwareFilteringSupported(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage, bool bPreciseFormatOnly = false) = 0;	

    public:
        virtual ResourceCreateOrRetrieveResult CreateOrRetrieve(uint32 nGroup, 
                                                                const String& strName, 
																const String& strGroupName, 
																bool bIsManualLoad = false,
																ResourceManualLoader* pManualLoader = nullptr, 
																const NameValuePairMap* pLoadParams = nullptr,
                                                                uint32 nUsage = TextureManager::ms_nUsage_Default,
																FTextureType eTexture = TextureManager::ms_eTexture_Default, 
                                                                FTextureFilterType eTextureFilter = TextureManager::ms_eTextureFilter_Default,
                                                                FTextureAddressingType eTextureAddressing = TextureManager::ms_eTextureAddressing_Default,
                                                                FTextureBorderColorType eTextureBorderColor = TextureManager::ms_eTextureBorderColor_Default,
                                                                FMSAASampleCountType eMSAASampleCount = TextureManager::ms_eMSAASampleCount_Default,
                                                                FPixelFormatType ePixelFormat = TextureManager::ms_ePixelFormat_Default, 
                                                                uint32 nWidth = TextureManager::ms_nWidth_Default,
                                                                uint32 nHeight = TextureManager::ms_nHeight_Default,
                                                                uint32 nDepth = TextureManager::ms_nDepth_Default,
                                                                uint16 nBitDepthInteger = TextureManager::ms_nBitDepthInteger_Default,
                                                                uint16 nBitDepthFloat = TextureManager::ms_nBitDepthFloat_Default,
                                                                bool bIsTreatLuminanceAsAlpha = TextureManager::ms_bIsTreatLuminanceAsAlpha_Default,
																int32 nMipMapsCount = TextureManager::ms_nMipMapsCount_Default, 
                                                                bool bIsMipMapsHardwareGenerated = TextureManager::ms_bIsMipMapsHardwareGenerated_Default,
																float fGamma = TextureManager::ms_fGamma_Default,
                                                                bool bIsGammaHardware = TextureManager::ms_bIsGammaHardware_Default);

    public:
        virtual Texture* Prepare(uint32 nGroup, 
                                 const String& strName, 
								 const String& strGroupName, 
								 uint32 nUsage = TextureManager::ms_nUsage_Default,
                                 FTextureType eTexture = TextureManager::ms_eTexture_Default, 
                                 FTextureFilterType eTextureFilter = TextureManager::ms_eTextureFilter_Default,
                                 FTextureAddressingType eTextureAddressing = TextureManager::ms_eTextureAddressing_Default,
                                 FTextureBorderColorType eTextureBorderColor = TextureManager::ms_eTextureBorderColor_Default,
                                 FMSAASampleCountType eMSAASampleCount = TextureManager::ms_eMSAASampleCount_Default,
                                 FPixelFormatType ePixelFormat = TextureManager::ms_ePixelFormat_Default, 
                                 uint32 nWidth = TextureManager::ms_nWidth_Default,
                                 uint32 nHeight = TextureManager::ms_nHeight_Default,
                                 uint32 nDepth = TextureManager::ms_nDepth_Default,
                                 uint16 nBitDepthInteger = TextureManager::ms_nBitDepthInteger_Default,
                                 uint16 nBitDepthFloat = TextureManager::ms_nBitDepthFloat_Default,
                                 bool bIsTreatLuminanceAsAlpha = TextureManager::ms_bIsTreatLuminanceAsAlpha_Default,
                                 int32 nMipMapsCount = TextureManager::ms_nMipMapsCount_Default, 
                                 bool bIsMipMapsHardwareGenerated = TextureManager::ms_bIsMipMapsHardwareGenerated_Default,
                                 float fGamma = TextureManager::ms_fGamma_Default,
                                 bool bIsGammaHardware = TextureManager::ms_bIsGammaHardware_Default);

		virtual Texture* Load(uint32 nGroup, 
                              const String& strName, 
                              const String& strGroupName, 
                              uint32 nUsage = TextureManager::ms_nUsage_Default,
                              FTextureType eTexture = TextureManager::ms_eTexture_Default, 
                              FTextureFilterType eTextureFilter = TextureManager::ms_eTextureFilter_Default,
                              FTextureAddressingType eTextureAddressing = TextureManager::ms_eTextureAddressing_Default,
                              FTextureBorderColorType eTextureBorderColor = TextureManager::ms_eTextureBorderColor_Default,
                              FMSAASampleCountType eMSAASampleCount = TextureManager::ms_eMSAASampleCount_Default,
                              FPixelFormatType ePixelFormat = TextureManager::ms_ePixelFormat_Default, 
                              uint32 nWidth = TextureManager::ms_nWidth_Default,
                              uint32 nHeight = TextureManager::ms_nHeight_Default,
                              uint32 nDepth = TextureManager::ms_nDepth_Default,
                              uint16 nBitDepthInteger = TextureManager::ms_nBitDepthInteger_Default,
                              uint16 nBitDepthFloat = TextureManager::ms_nBitDepthFloat_Default,
                              bool bIsTreatLuminanceAsAlpha = TextureManager::ms_bIsTreatLuminanceAsAlpha_Default,
                              int32 nMipMapsCount = TextureManager::ms_nMipMapsCount_Default, 
                              bool bIsMipMapsHardwareGenerated = TextureManager::ms_bIsMipMapsHardwareGenerated_Default,
                              float fGamma = TextureManager::ms_fGamma_Default,
                              bool bIsGammaHardware = TextureManager::ms_bIsGammaHardware_Default);

    public:
        Texture* CreateTexture(uint32 nGroup, 
                               const String& strName,
                               const String& strGroupName, 
                               FTextureType eTexture = F_Texture_2D, 
                               int32 nMipMapsCount = E_TextureMipMap_Default, 
						       float fGamma = 1.0f, 
                               bool bIsTreatLuminanceAsAlpha = false, 
                               FPixelFormatType ePixelFormat = F_PixelFormat_Unknown, 
                               bool bUseMemoryImage = false,
						       bool bBackground = true, 
                               uint16 nRecoveryGroupID = 0);

		Texture* CreateTextureManual(uint32 nGroup,
                                     const String& strName,  
                                     const String& strGroupName, 
                                     ResourceManualLoader* pManualLoader,
                                     FTextureType eTexture, 
                                     uint32 nWidth, 
                                     uint32 nHeight, 
                                     uint32 nDepth, 
						             int32 nMipMapsCount, 
                                     FPixelFormatType ePixelFormat, 
                                     uint32 nUsage = E_TextureUsage_Default, 
                                     bool bUseMemoryImage = false);

		Texture* CreateTextureFromImage(uint32 nGroup,
                                        const String& strName, 
                                        const String& strGroupName, 
                                        Image* pImage, 
                                        FTextureType eTexture = F_Texture_2D,
						                int32 nMipMapsCount = E_TextureMipMap_Default, 
                                        float fGamma = 1.0f, 
                                        bool bIsTreatLuminanceAsAlpha = false,
						                FPixelFormatType ePixelFormat = F_PixelFormat_Unknown,
                                        bool bUseMemoryImage = false);
		
		Texture* CreateTextureFromDDSImage(uint32 nGroup,
                                           const String& strName, 
                                           const String& strGroupName, 
                                           FFileMemory* pInput, 
                                           FTextureType eTexture = F_Texture_2D,
						                   int32 nMipMapsCount = E_TextureMipMap_Default, 
                                           float fGamma = 1.0f, 
                                           bool bIsTreatLuminanceAsAlpha = false,
						                   FPixelFormatType ePixelFormat = F_PixelFormat_Unknown, 
                                           bool bUseMemoryImage = false);
	
		Texture* CreateTextureFromRawData(uint32 nGroup, 
                                          const String& strName, 
                                          const String& strGroupName, 
                                          FFileMemory* pInput, 
                                          uint32 nWidth, 
                                          uint32 nHeight, 
						                  FPixelFormatType ePixelFormat, 
                                          FTextureType eTexture = F_Texture_2D, 
                                          int32 nMipMapsCount = E_TextureMipMap_Default, 
                                          float fGamma = 1.0f, 
                                          bool bUseMemoryImage = false);

    protected:

    };

}; //LostPeterEngine

#endif