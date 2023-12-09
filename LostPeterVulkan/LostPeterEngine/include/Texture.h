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

		FPixelFormatType m_ePixelFormat;
		size_t m_nWidth;
		size_t m_nHeight;
		size_t m_nDepth;
		size_t m_nSize;
		
		FPixelFormatType m_ePixelFormatSrc;
		size_t m_nSrcWidth;
		size_t m_nSrcHeight;
		size_t m_nSrcDepth;

		FPixelFormatType m_ePixelFormatDesired;
		uint16 m_nDesiredIntegerBitDepth;
		uint16 m_nDesiredFloatBitDepth;
		bool m_bTreatLuminanceAsAlpha;

		size_t m_nNumRequestedMipMaps;
		size_t m_nNumMipMaps;
		bool m_bMipMapsHardwareGenerated;

		float m_fGamma;
		uint32 m_nFSAA;

		bool m_bInternalResourcesCreated;

	public:
		F_FORCEINLINE FTextureType GetTextureType() const { return m_eTexture; }
		F_FORCEINLINE void SetTextureType(FTextureType eTexture) { m_eTexture = eTexture; }
		F_FORCEINLINE uint32 GetUsage() const { return m_nUsage; }
		F_FORCEINLINE void SetUsage(uint32 nUsage) { m_nUsage = nUsage; }
		F_FORCEINLINE bool	IsLoaded() const { return m_bIsLoaded; }
		F_FORCEINLINE bool	IsManual() const { return m_bIsManual; }

		F_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		F_FORCEINLINE void SetPixelFormat(FPixelFormatType ePixelFormat)	
		{ 
			m_ePixelFormat = ePixelFormat; 
			m_ePixelFormatSrc = ePixelFormat;	
			m_ePixelFormatDesired = ePixelFormat;
		}
		F_FORCEINLINE size_t GetWidth() const { return m_nWidth; }
		F_FORCEINLINE void SetWidth(size_t nW) { m_nWidth = m_nSrcWidth = nW; }
		F_FORCEINLINE size_t GetHeight() const { return m_nHeight; }
		F_FORCEINLINE void SetHeight(size_t nH) { m_nHeight = m_nSrcHeight = nH; }
		F_FORCEINLINE size_t GetDepth() const { return m_nDepth; }
		F_FORCEINLINE void SetDepth(size_t nD) { m_nDepth = m_nSrcDepth=nD; }
		F_FORCEINLINE size_t GetSize() const { return m_nSize; }

		F_FORCEINLINE FPixelFormatType GetSrcPixelFormat() const { return m_ePixelFormatSrc; }
		F_FORCEINLINE size_t GetSrcWidth() const { return m_nSrcWidth; }
		F_FORCEINLINE size_t GetSrcHeight() const { return m_nSrcHeight; }
		F_FORCEINLINE size_t GetSrcDepth() const { return m_nSrcDepth; }
		
		F_FORCEINLINE FPixelFormatType GetDesiredPixelFormat() const { return m_ePixelFormatDesired; }
		F_FORCEINLINE uint16 GetDesiredIntegerBitDepth() const { return m_nDesiredIntegerBitDepth; }
		F_FORCEINLINE void SetDesiredIntegerBitDepth(uint16 n) { m_nDesiredIntegerBitDepth = n; }
		F_FORCEINLINE uint16 GetDesiredFloatBitDepth() const { return m_nDesiredFloatBitDepth; }
		F_FORCEINLINE void SetDesiredFloatBitDepth(uint16 n) { m_nDesiredFloatBitDepth = n; }
		F_FORCEINLINE void SetDesiredBitDepths(uint16 nI, uint16 nF) 
		{
			m_nDesiredIntegerBitDepth = nI;
			m_nDesiredFloatBitDepth = nF;
		}
		F_FORCEINLINE bool GetTreatLuminanceAsAlpha() const { return m_bTreatLuminanceAsAlpha; }
		F_FORCEINLINE void SetTreatLuminanceAsAlpha(bool b) { m_bTreatLuminanceAsAlpha = b; }
		
		F_FORCEINLINE size_t GetNumRequestedMipMaps() const { return m_nNumRequestedMipMaps; }
		F_FORCEINLINE void SetNumRequestedMipMaps(size_t nMipMaps) { m_nNumRequestedMipMaps = nMipMaps; }
		F_FORCEINLINE size_t GetNumMipMaps() const	{ return m_nNumMipMaps; }
		F_FORCEINLINE void SetNumMipMaps(size_t nMipMaps) { m_nNumMipMaps = nMipMaps; }
		F_FORCEINLINE bool IsMipMapsHardwareGenerated() const { return m_bMipMapsHardwareGenerated; }
			
		F_FORCEINLINE float GetGamma() const { return m_fGamma; }
		F_FORCEINLINE void SetGamma(float fGamma) { m_fGamma = fGamma; }
		F_FORCEINLINE uint32 GetFSAA() const { return m_nFSAA; }
		F_FORCEINLINE void SetFSAA(uint32 n) { m_nFSAA = n; }
		
	public:
		virtual size_t GetNumFaces() const;
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