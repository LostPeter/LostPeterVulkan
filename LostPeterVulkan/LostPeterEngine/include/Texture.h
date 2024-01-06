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
		FTextureType m_eTexture;
		uint32 m_nUsage;
		bool m_bIsLoaded;
		bool m_bIsManual;

		FPixelFormatType m_ePixelFormatDesired;
		FPixelFormatType m_ePixelFormat;	
		uint32 m_nWidth;
		uint32 m_nHeight;
		uint32 m_nDepth;
		uint32 m_nSize;
		
		FPixelFormatType m_ePixelFormatSrc;
		uint32 m_nWidthSrc;
		uint32 m_nHeightSrc;
		uint32 m_nDepthSrc;

		uint16 m_nBitDepthIntegerDesired;
		uint16 m_nBitDepthFloatDesired;
		bool m_bTreatLuminanceAsAlpha;

		uint32 m_nMipMapsCountRequested;
		uint32 m_nMipMapsCount;
		bool m_bMipMapsHardwareGenerated;

		float m_fGamma;
		bool m_bIsHardwareGamma;
		uint32 m_nFSAA;

		bool m_bInternalResourcesCreated;

	public:
		F_FORCEINLINE FTextureType GetTextureType() const { return m_eTexture; }
		F_FORCEINLINE void SetTextureType(FTextureType eTexture) { m_eTexture = eTexture; }
		F_FORCEINLINE uint32 GetUsage() const { return m_nUsage; }
		F_FORCEINLINE void SetUsage(uint32 nUsage) { m_nUsage = nUsage; }
		F_FORCEINLINE bool IsLoaded() const { return m_bIsLoaded; }
		F_FORCEINLINE bool IsManual() const { return m_bIsManual; }

		F_FORCEINLINE FPixelFormatType GetDesiredPixelFormat() const { return m_ePixelFormatDesired; }
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
		F_FORCEINLINE uint32 GetSize() const { return m_nSize; }

		F_FORCEINLINE FPixelFormatType GetPixelFormatSrc() const { return m_ePixelFormatSrc; }
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
		F_FORCEINLINE bool GetTreatLuminanceAsAlpha() const { return m_bTreatLuminanceAsAlpha; }
		F_FORCEINLINE void SetTreatLuminanceAsAlpha(bool bTreatLuminanceAsAlpha) { m_bTreatLuminanceAsAlpha = bTreatLuminanceAsAlpha; }
		
		F_FORCEINLINE uint32 GetMipMapsCountRequested() const { return m_nMipMapsCountRequested; }
		F_FORCEINLINE void SetMipMapsCountRequested(uint32 nMipMapsCountRequested) { m_nMipMapsCountRequested = nMipMapsCountRequested; }
		F_FORCEINLINE uint32 GetMipMapsCount() const { return m_nMipMapsCount; }
		F_FORCEINLINE void SetMipMapsCount(uint32 nMipMapsCount) { m_nMipMapsCount = nMipMapsCount; }
		F_FORCEINLINE bool IsMipMapsHardwareGenerated() const { return m_bMipMapsHardwareGenerated; }
			
		F_FORCEINLINE float GetGamma() const { return m_fGamma; }
		F_FORCEINLINE void SetGamma(float fGamma) { m_fGamma = fGamma; }
		F_FORCEINLINE bool IsHardwareGammaEnabled() const { return m_bIsHardwareGamma; }
		F_FORCEINLINE void SetHardwareGammaEnabled(bool bIsHardwareGamma) { m_bIsHardwareGamma = bIsHardwareGamma; }
		F_FORCEINLINE uint32 GetFSAA() const { return m_nFSAA; }
		F_FORCEINLINE void SetFSAA(uint32 nFSAA) { m_nFSAA = nFSAA; }
		
	public:
		virtual uint32 GetFacesCount() const;
	    virtual bool HasAlpha() const;
		virtual uint32 CalculateSize() const;

		virtual bool LoadFromRawData(FFileMemory* pInput, uint32 nWidth, uint32 nHeight, FPixelFormatType ePixelFormat);
		virtual bool LoadFromImage(Image* pImage);
		virtual bool LoadFromImages(const ImagePtrVector& aImages);
		virtual bool LoadFromDDSImage(FFileMemory* pInput) { return false; }

		virtual bool SaveTextureToFile(const char* szFileName) { return false; }
		
		virtual bool CreateInternalResources();
        virtual void FreeInternalResources();

		virtual StreamTexture* GetTextureStream(uint32 nFace = 0, uint32 nMipMap = 0) = 0;

	protected:
		virtual void createInternalResourcesImpl() = 0;
		virtual void freeInternalResourcesImpl() = 0;
    };

}; //LostPeterEngine

#endif