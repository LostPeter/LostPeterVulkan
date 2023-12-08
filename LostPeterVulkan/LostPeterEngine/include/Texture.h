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
		E_FORCEINLINE FTextureType GetTextureType() const { return m_eTexture; }
		E_FORCEINLINE void SetTextureType(FTextureType eTexture) { m_eTexture = eTexture; }
		E_FORCEINLINE uint32 GetUsage() const { return m_nUsage; }
		E_FORCEINLINE void SetUsage(uint32 nUsage) { m_nUsage = nUsage; }
		E_FORCEINLINE bool	IsLoaded() const { return m_bIsLoaded; }
		E_FORCEINLINE bool	IsManual() const { return m_bIsManual; }

		E_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		E_FORCEINLINE void SetPixelFormat(FPixelFormatType ePixelFormat)	
		{ 
			m_ePixelFormat = ePixelFormat; 
			m_ePixelFormatSrc = ePixelFormat;	
			m_ePixelFormatDesired = ePixelFormat;
		}
		E_FORCEINLINE size_t GetWidth() const { return m_nWidth; }
		E_FORCEINLINE void SetWidth(size_t nW) { m_nWidth = m_nSrcWidth = nW; }
		E_FORCEINLINE size_t GetHeight() const { return m_nHeight; }
		E_FORCEINLINE void SetHeight(size_t nH) { m_nHeight = m_nSrcHeight = nH; }
		E_FORCEINLINE size_t GetDepth() const { return m_nDepth; }
		E_FORCEINLINE void SetDepth(size_t nD) { m_nDepth = m_nSrcDepth=nD; }
		E_FORCEINLINE size_t GetSize() const { return m_nSize; }

		E_FORCEINLINE FPixelFormatType GetSrcPixelFormat() const { return m_ePixelFormatSrc; }
		E_FORCEINLINE size_t GetSrcWidth() const { return m_nSrcWidth; }
		E_FORCEINLINE size_t GetSrcHeight() const { return m_nSrcHeight; }
		E_FORCEINLINE size_t GetSrcDepth() const { return m_nSrcDepth; }
		
		E_FORCEINLINE FPixelFormatType GetDesiredPixelFormat() const { return m_ePixelFormatDesired; }
		E_FORCEINLINE uint16 GetDesiredIntegerBitDepth() const { return m_nDesiredIntegerBitDepth; }
		E_FORCEINLINE void SetDesiredIntegerBitDepth(uint16 n) { m_nDesiredIntegerBitDepth = n; }
		E_FORCEINLINE uint16 GetDesiredFloatBitDepth() const { return m_nDesiredFloatBitDepth; }
		E_FORCEINLINE void SetDesiredFloatBitDepth(uint16 n) { m_nDesiredFloatBitDepth = n; }
		E_FORCEINLINE void SetDesiredBitDepths(uint16 nI, uint16 nF) 
		{
			m_nDesiredIntegerBitDepth = nI;
			m_nDesiredFloatBitDepth = nF;
		}
		E_FORCEINLINE bool GetTreatLuminanceAsAlpha() const { return m_bTreatLuminanceAsAlpha; }
		E_FORCEINLINE void SetTreatLuminanceAsAlpha(bool b) { m_bTreatLuminanceAsAlpha = b; }
		
		E_FORCEINLINE size_t GetNumRequestedMipMaps() const { return m_nNumRequestedMipMaps; }
		E_FORCEINLINE void SetNumRequestedMipMaps(size_t nMipMaps) { m_nNumRequestedMipMaps = nMipMaps; }
		E_FORCEINLINE size_t GetNumMipMaps() const	{ return m_nNumMipMaps; }
		E_FORCEINLINE void SetNumMipMaps(size_t nMipMaps) { m_nNumMipMaps = nMipMaps; }
		E_FORCEINLINE bool IsMipMapsHardwareGenerated() const { return m_bMipMapsHardwareGenerated; }
			
		E_FORCEINLINE float GetGamma() const { return m_fGamma; }
		E_FORCEINLINE void SetGamma(float fGamma) { m_fGamma = fGamma; }
		E_FORCEINLINE uint32 GetFSAA() const { return m_nFSAA; }
		E_FORCEINLINE void SetFSAA(uint32 n) { m_nFSAA = n; }
		
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