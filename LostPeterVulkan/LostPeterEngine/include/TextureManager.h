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

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport TextureManager : public FSingleton<TextureManager>
                                      , public Base
    {
    public:
        TextureManager();
        virtual ~TextureManager();

    public:
    protected:
        TextureSerializer* m_pTextureSerializer;
        TexturePtrVector m_aTexture;
        TextureGroupPtrMap m_mapTextureGroup;

        uint16 m_nBitDepthIntegerPreferred;
		uint16 m_nBitDepthFloatPreferred;
		size_t m_nNumMipMapsDefault;
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
      
        virtual size_t GetNumMipMapsDefault() { return m_nNumMipMapsDefault; }
		virtual void SetNumMipMapsDefault(size_t nNumMipMapsDefault) { m_nNumMipMapsDefault = nNumMipMapsDefault; }
		virtual float GetMipMapLODBiasDefault() { return m_fMipMapLODBiasDefault; }
		virtual void SetMipMapLODBiasDefault(float fMipMapLODBiasDefault) { m_fMipMapLODBiasDefault = fMipMapLODBiasDefault; }
       
		virtual bool IsPixelFormatSupported(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage);
		virtual bool IsPixelFormatSupportedEquivalent(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage);

		virtual FPixelFormatType GetPixelFormatNative(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage) = 0;
		virtual bool IsHardwareFilteringSupported(FTextureType eTexture, FPixelFormatType ePixelFormat, uint32 nUsage, bool bPreciseFormatOnly = false) = 0;	

    public:
        Texture* CreateTexture(uint32 nGroup, 
                               const String& strName,
                               FTextureType eTexture = F_Texture_2D, 
                               int nNumMipMaps = E_TextureMipMap_Default, 
						       float fGamma = 1.0f, 
                               bool bIsAlpha = false, 
                               FPixelFormatType ePixelFormatDesired = F_PixelFormat_Unknown, 
                               bool bUseMemoryImage = false,
						       bool bBackground = true, 
                               uint16 nRecoveryGroupID = 0);

		Texture* CreateTextureManual(uint32 nGroup,
                                     const String& strName,  
                                     FTextureType eTexture, 
                                     uint32 nWidth, 
                                     uint32 nHeight, 
                                     uint32 nDepth, 
						             int nNumMipMaps, 
                                     FPixelFormatType ePixelFormat, 
                                     uint32 nUsage = E_TextureUsage_Default, 
                                     bool bUseMemoryImage = false, 
                                     uint32 nFSAA = 0);

		Texture* CreateTextureFromImage(uint32 nGroup,
                                        const String& strName, 
                                        Image* pImage, 
                                        FTextureType eTexture = F_Texture_2D,
						                int nNumMipMaps = E_TextureMipMap_Default, 
                                        float fGamma = 1.0f, 
                                        bool bIsAlpha = false,
						                FPixelFormatType ePixelFormatDesired = F_PixelFormat_Unknown, 
                                        bool bUseMemoryImage = false);
		
		Texture* CreateTextureFromDDSImage(uint32 nGroup,
                                           const String& strName, 
                                           FFileMemory* pInput, 
                                           FTextureType eTexture = F_Texture_2D,
						                   int nNumMipMaps = E_TextureMipMap_Default, 
                                           float fGamma = 1.0f, 
                                           bool bIsAlpha = false,
						                   FPixelFormatType ePixelFormatDesired = F_PixelFormat_Unknown, 
                                           bool bUseMemoryImage = false);
	
		Texture* CreateTextureFromRawData(uint32 nGroup, 
                                          const String& strName, 
                                          FFileMemory* pInput, 
                                          size_t nWidth, 
                                          size_t nHeight, 
						                  FPixelFormatType ePixelFormat, 
                                          FTextureType eTexture = F_Texture_2D, 
                                          int nNumMipMaps = E_TextureMipMap_Default, 
                                          float fGamma = 1.0f, 
                                          bool bUseMemoryImage = false);

    protected:
		virtual Texture* createImpl(uint32 nGroup, const String& strName, const String2StringMap* pParams = nullptr) = 0;
    };

}; //LostPeterEngine

#endif