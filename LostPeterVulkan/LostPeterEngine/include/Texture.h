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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Resource.h"

namespace LostPeterEngine
{
    class engineExport Texture : public Resource
    {
    public:
        Texture(ResourceManager* pResourceManager,
				uint32 nGroup, 
				const String& strName,
				const String& strGroupName,
                ResourceHandle nHandle,
                bool bIsManualLoad = false,
                ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~Texture();

    public:
	protected:
		uint32 m_nUsage;
		FTextureType m_eTexture;
		FTextureFilterType m_eTextureFilter;
        FTextureAddressingType m_eTextureAddressing;
        FTextureBorderColorType m_eTextureBorderColor;
		FMSAASampleCountType m_eMSAASampleCount;

		FPixelFormatType m_ePixelFormatDesired;
		FPixelFormatType m_ePixelFormatSrc;
		FPixelFormatType m_ePixelFormat;	
		uint32 m_nWidth;
		uint32 m_nHeight;
		uint32 m_nDepth;
		uint32 m_nWidthSrc;
		uint32 m_nHeightSrc;
		uint32 m_nDepthSrc;

		uint16 m_nBitDepthIntegerDesired;
		uint16 m_nBitDepthFloatDesired;
		bool m_bIsTreatLuminanceAsAlpha;

		uint32 m_nMipMapsCountRequested;
		uint32 m_nMipMapsCount;
		bool m_bIsMipMapsHardwareGenerated;

		float m_fGamma;
		bool m_bIsGammaHardware;

		bool m_bInternalResourcesCreated;

	public:
		F_FORCEINLINE uint32 GetUsage() const { return m_nUsage; }
		F_FORCEINLINE void SetUsage(uint32 nUsage) { m_nUsage = nUsage; }
		F_FORCEINLINE bool IsUsage_Static() const { return m_nUsage & E_TextureUsage_Static; }
		F_FORCEINLINE bool IsUsage_Dynamic() const { return m_nUsage & E_TextureUsage_Dynamic; }
		F_FORCEINLINE bool IsUsage_WriteOnly() const { return m_nUsage & E_TextureUsage_WriteOnly; }
		F_FORCEINLINE bool IsUsage_StaticWriteOnly() const { return m_nUsage & E_TextureUsage_StaticWriteOnly; }
		F_FORCEINLINE bool IsUsage_DynamicWriteOnly() const { return m_nUsage & E_TextureUsage_DynamicWriteOnly; }
		F_FORCEINLINE bool IsUsage_DynamicWriteOnlyDiscardable() const { return m_nUsage & E_TextureUsage_DynamicWriteOnlyDiscardable; }
		F_FORCEINLINE bool IsUsage_AutoMipMap() const { return m_nUsage & E_TextureUsage_AutoMipMap; }
		F_FORCEINLINE bool IsUsage_RenderTarget() const { return m_nUsage & E_TextureUsage_RenderTarget; }
		F_FORCEINLINE bool IsUsage_NotSRV() const { return m_nUsage & E_TextureUsage_NotSRV; }
		F_FORCEINLINE bool IsUsage_UnorderedAccessView() const { return m_nUsage & E_TextureUsage_UnorderedAccessView; }
		F_FORCEINLINE bool IsUsage_UnorderedAccessViewNotSRV() const { return m_nUsage & E_TextureUsage_UnorderedAccessViewNotSRV; }
		F_FORCEINLINE bool IsUsage_Default() const { return m_nUsage & E_TextureUsage_Default; }
		F_FORCEINLINE FTextureType GetTextureType() const { return m_eTexture; }
		F_FORCEINLINE void SetTextureType(FTextureType eTexture) { m_eTexture = eTexture; }
		F_FORCEINLINE FTextureFilterType GetTextureFilterType() const { return m_eTextureFilter; }
		F_FORCEINLINE void SetTextureFilterType(FTextureFilterType eTextureFilter) { m_eTextureFilter = eTextureFilter; }
		F_FORCEINLINE FTextureAddressingType GetTextureAddressingType() const { return m_eTextureAddressing; }
		F_FORCEINLINE void SetTextureAddressingType(FTextureAddressingType eTextureAddressing) { m_eTextureAddressing = eTextureAddressing; }
		F_FORCEINLINE FTextureBorderColorType GetTextureBorderColorType() const { return m_eTextureBorderColor; }
		F_FORCEINLINE void SetTextureBorderColorType(FTextureBorderColorType eTextureBorderColor) { m_eTextureBorderColor = eTextureBorderColor; }
		F_FORCEINLINE FMSAASampleCountType GetMSAASampleCountType() const { return m_eMSAASampleCount; }
		F_FORCEINLINE void SetMSAASampleCountType(FMSAASampleCountType eMSAASampleCount) { m_eMSAASampleCount = eMSAASampleCount; }

		F_FORCEINLINE FPixelFormatType GetDesiredPixelFormat() const { return m_ePixelFormatDesired; }
		F_FORCEINLINE FPixelFormatType GetPixelFormatSrc() const { return m_ePixelFormatSrc; }
		F_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		F_FORCEINLINE void SetPixelFormat(FPixelFormatType ePixelFormat)	
		{ 
			m_ePixelFormatDesired = ePixelFormat;
			m_ePixelFormat = ePixelFormat; 
			m_ePixelFormatSrc = ePixelFormat;	
		}
		F_FORCEINLINE uint32 GetWidth() const { return m_nWidth; }
		F_FORCEINLINE void SetWidth(uint32 nWidth) { m_nWidth = m_nWidthSrc = nWidth; }
		F_FORCEINLINE uint32 GetHeight() const { return m_nHeight; }
		F_FORCEINLINE void SetHeight(uint32 nHeight) { m_nHeight = m_nHeightSrc = nHeight; }
		F_FORCEINLINE uint32 GetDepth() const { return m_nDepth; }
		F_FORCEINLINE void SetDepth(uint32 nDepth) { m_nDepth = m_nDepthSrc = nDepth; }
		F_FORCEINLINE uint32 GetWidthSrc() const { return m_nWidthSrc; }
		F_FORCEINLINE uint32 GetHeightSrc() const { return m_nHeightSrc; }
		F_FORCEINLINE uint32 GetDepthSrc() const { return m_nDepthSrc; }
		
		F_FORCEINLINE uint16 GetBitDepthIntegerDesired() const { return m_nBitDepthIntegerDesired; }
		F_FORCEINLINE void SetBitDepthIntegerDesired(uint16 nBitDepthIntegerDesired) { m_nBitDepthIntegerDesired = nBitDepthIntegerDesired; }
		F_FORCEINLINE uint16 GetBitDepthFloatDesired() const { return m_nBitDepthFloatDesired; }
		F_FORCEINLINE void SetBitDepthFloatDesired(uint16 nBitDepthFloatDesired) { m_nBitDepthFloatDesired = nBitDepthFloatDesired; }
		F_FORCEINLINE void SetBitDepthsDesired(uint16 nBitDepthIntegerDesired, uint16 nBitDepthFloatDesired) 
		{
			m_nBitDepthIntegerDesired = nBitDepthIntegerDesired;
			m_nBitDepthFloatDesired = nBitDepthFloatDesired;
		}
		F_FORCEINLINE bool IsTreatLuminanceAsAlpha() const { return m_bIsTreatLuminanceAsAlpha; }
		F_FORCEINLINE void SetIsTreatLuminanceAsAlpha(bool bIsTreatLuminanceAsAlpha) { m_bIsTreatLuminanceAsAlpha = bIsTreatLuminanceAsAlpha; }
		
		F_FORCEINLINE uint32 GetMipMapsCountRequested() const { return m_nMipMapsCountRequested; }
		F_FORCEINLINE void SetMipMapsCountRequested(uint32 nMipMapsCountRequested) { m_nMipMapsCountRequested = nMipMapsCountRequested; }
		F_FORCEINLINE uint32 GetMipMapsCount() const { return m_nMipMapsCount; }
		F_FORCEINLINE void SetMipMapsCount(uint32 nMipMapsCount) { m_nMipMapsCount = nMipMapsCount; }
		F_FORCEINLINE bool IsMipMapsHardwareGenerated() const { return m_bIsMipMapsHardwareGenerated; }
		F_FORCEINLINE void SetIsMipMapsHardwareGenerated(bool bIsMipMapsHardwareGenerated) { m_bIsMipMapsHardwareGenerated = bIsMipMapsHardwareGenerated; }
			
		F_FORCEINLINE float GetGamma() const { return m_fGamma; }
		F_FORCEINLINE void SetGamma(float fGamma) { m_fGamma = fGamma; }
		F_FORCEINLINE bool IsGammaHardware() const { return m_bIsGammaHardware; }
		F_FORCEINLINE void SetIsGammaHardware(bool bIsGammaHardware) { m_bIsGammaHardware = bIsGammaHardware; }

	public:
        virtual void Destroy();
		
	public:
		virtual uint32 GetFacesCount() const;
	    virtual bool HasAlpha() const;

		virtual bool LoadFromRawData(FFileMemory* pInput, uint32 nWidth, uint32 nHeight, FPixelFormatType ePixelFormat);
		virtual bool LoadFromImage(Image* pImage);
		virtual bool LoadFromImages(const ImagePtrVector& aImages);
		virtual bool LoadFromDDSImage(FFileMemory* pInput) { return false; }

		virtual bool SaveTextureToFile(const char* szFileName) { return false; }
		
		virtual bool CreateInternalResources();
        virtual void FreeInternalResources();

		virtual StreamTexture* GetTextureStream(uint32 nFace = 0, uint32 nMipMap = 0) = 0;

	protected:
		virtual uint32 calculateSize() const;

	protected:
		virtual void createInternalResourcesImpl() = 0;
		virtual void freeInternalResourcesImpl() = 0;
    };

}; //LostPeterEngine

#endif