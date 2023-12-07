/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport Image
	{
	public:
		Image();
		Image(const Image& image);
		virtual ~Image();

		Image& operator =(const Image& image);

	public:
	protected:
		size_t m_nWidth;
		size_t m_nHeight;
		size_t m_nDepth;
		size_t m_nSize;		
		size_t m_nNumMipMaps;	
		uint32 m_nFlags;		
		FPixelFormatType m_ePixelFormat;	
		uint8 m_nPixelSize;	
		uint8* m_pBuffer;
		bool m_bAutoDelete;

	public:
		E_FORCEINLINE uint8* GetData() { return m_pBuffer; }
		E_FORCEINLINE const uint8* GetData() const { return m_pBuffer; }

		E_FORCEINLINE size_t GetSize() const { return m_nSize; }
		E_FORCEINLINE size_t GetNumMipMaps() const { return m_nNumMipMaps; }
		E_FORCEINLINE bool	HasFlag(const EImageFlagType imgFlag) const { return m_nFlags & imgFlag ? true : false; }
		E_FORCEINLINE size_t GetWidth() const { return m_nWidth; }
		E_FORCEINLINE size_t GetHeight() const { return m_nHeight; }
		E_FORCEINLINE size_t GetDepth() const { return m_nDepth; }
		E_FORCEINLINE size_t GetNumFaces() const { return HasFlag(E_ImageFlag_IsCubeMap) ? 6 : 1;}
		E_FORCEINLINE size_t GetRowSpan() const { return m_nWidth * m_nPixelSize; }
		E_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		E_FORCEINLINE uint8 GetBPP() const { return m_nPixelSize * 8; }
		bool GetHasAlpha() const;
		void DestroyData();

	public:
		bool LoadDynamicImage(uint8* pData, 
                              size_t uWidth, 
                              size_t uHeight, 
                              size_t uDepth, 
                              FPixelFormatType typePixelFormat, 
							  bool autoDelete = false, 
                              size_t numFaces = 1, 
                              size_t numMipMaps = 0);
		bool LoadDynamicImage(uint8* pData, 
                              size_t uWidth, 
                              size_t uHeight, 
                              FPixelFormatType typePixelFormat)
		{
			return LoadDynamicImage(pData, 
                                    uWidth, 
                                    uHeight, 
                                    1, 
                                    typePixelFormat);
		}

		bool LoadRawData(FFileMemory* pInput, 
                         size_t uWidth, 
                         size_t uHeight, 
                         size_t uDepth, 
                         FPixelFormatType typePixelFormat,
						 size_t numFaces = 1, 
                         size_t numMipMaps = 0);
		bool LoadRawData(FFileMemory* pInput, 
                         size_t uWidth, 
                         size_t uHeight, 
                         FPixelFormatType typePixelFormat)
		{
			return LoadRawData(pInput, 
                               uWidth, 
                               uHeight, 
                               1, 
                               typePixelFormat);
		}
		
		bool Load(const String& strName, uint32 nGroup);
		bool Load(const String& strFilePath);
		bool Load(FFileIO* pFIO, const String& strType = FUtilString::BLANK);
		bool Load(FFileMemory* pInput, const String& strType = FUtilString::BLANK);
		bool Save(const String& strFilePath);
		FFileMemory* Encode(const String& strExt);
	
		FColor GetColorAt(int x, int y, int z = -1) const;
		bool GetPixelBox(FPixelBox& retPB, size_t face = 0, size_t mipmap = 0) const;

		bool FlipAroundY();
		bool FlipAroundX();

		static bool	Scale(const FPixelBox& src, const FPixelBox& dst, EImageFilterType typeImageFilter = E_ImageFilter_Bilinear);
		bool Resize(uint16 width, uint16 height, EImageFilterType typeImageFilter = E_ImageFilter_Bilinear);

		static size_t CalculateSize(size_t mipmaps, size_t faces, size_t width, size_t height, size_t depth, FPixelFormatType format);
		static String GetFileExtFromMagic(FFileIO* pInput);
		static bool	ApplyGamma(uint8* buffer, float gamma, size_t size, uint8 bpp);
	};

}; //LostPeterEngine

#endif