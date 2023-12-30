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

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Texture : public Base
    {
    public:
        Texture(uint32 nGroup, const String& strName);
        virtual ~Texture();

    public:
	protected:
		FTextureType m_eTexture;
		uint32 m_nUsage;
		bool m_bIsLoaded;
		bool m_bIsManual;

		FPixelFormatType m_ePixelFormatDesired;
		FPixelFormatType m_ePixelFormat;	
		size_t m_nWidth;
		size_t m_nHeight;
		size_t m_nDepth;
		size_t m_nSize;
		
		FPixelFormatType m_ePixelFormatSrc;
		size_t m_nWidthSrc;
		size_t m_nHeightSrc;
		size_t m_nDepthSrc;

		uint16 m_nBitDepthIntegerDesired;
		uint16 m_nBitDepthFloatDesired;
		bool m_bTreatLuminanceAsAlpha;

		size_t m_nMipMapsCountRequested;
		size_t m_nMipMapsCount;
		bool m_bMipMapsHardwareGenerated;

		float m_fGamma;
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
		F_FORCEINLINE size_t GetWidth() const { return m_nWidth; }
		F_FORCEINLINE void SetWidth(size_t nWidth) { m_nWidth = m_nWidthSrc = nWidth; }
		F_FORCEINLINE size_t GetHeight() const { return m_nHeight; }
		F_FORCEINLINE void SetHeight(size_t nHeight) { m_nHeight = m_nHeightSrc = nHeight; }
		F_FORCEINLINE size_t GetDepth() const { return m_nDepth; }
		F_FORCEINLINE void SetDepth(size_t nDepth) { m_nDepth = m_nDepthSrc = nDepth; }
		F_FORCEINLINE size_t GetSize() const { return m_nSize; }

		F_FORCEINLINE FPixelFormatType GetPixelFormatSrc() const { return m_ePixelFormatSrc; }
		F_FORCEINLINE size_t GetWidthSrc() const { return m_nWidthSrc; }
		F_FORCEINLINE size_t GetHeightSrc() const { return m_nHeightSrc; }
		F_FORCEINLINE size_t GetDepthSrc() const { return m_nDepthSrc; }
		
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
		
		F_FORCEINLINE size_t GetMipMapsCountRequested() const { return m_nMipMapsCountRequested; }
		F_FORCEINLINE void SetMipMapsCountRequested(size_t nMipMapsCountRequested) { m_nMipMapsCountRequested = nMipMapsCountRequested; }
		F_FORCEINLINE size_t GetMipMapsCount() const { return m_nMipMapsCount; }
		F_FORCEINLINE void SetMipMapsCount(size_t nMipMapsCount) { m_nMipMapsCount = nMipMapsCount; }
		F_FORCEINLINE bool IsMipMapsHardwareGenerated() const { return m_bMipMapsHardwareGenerated; }
			
		F_FORCEINLINE float GetGamma() const { return m_fGamma; }
		F_FORCEINLINE void SetGamma(float fGamma) { m_fGamma = fGamma; }
		F_FORCEINLINE uint32 GetFSAA() const { return m_nFSAA; }
		F_FORCEINLINE void SetFSAA(uint32 nFSAA) { m_nFSAA = nFSAA; }
		
	public:
		virtual size_t GetFacesCount() const;
	    virtual bool HasAlpha() const;
		virtual size_t CalculateSize() const;

		virtual bool LoadFromRawData(FFileMemory* pInput, size_t nWidth, size_t nHeight, FPixelFormatType ePixelFormat);
		virtual bool LoadFromImage(Image* pImage);
		virtual bool LoadFromImages(const ImagePtrVector& aImages);
		virtual bool LoadFromDDSImage(FFileMemory* pInput) { return false; }

		virtual bool SaveTextureToFile(const char* szFileName) { return false; }
		
		virtual bool CreateInternalResources();
        virtual void FreeInternalResources();

		virtual StreamTexture* GetTextureStream(size_t nFace = 0, size_t nMipMap = 0) = 0;

		virtual bool Load() = 0;
		virtual bool Unload() = 0;

	protected:
		virtual void createInternalResourcesImpl() = 0;
		virtual void freeInternalResourcesImpl() = 0;
    };

}; //LostPeterEngine

#endif