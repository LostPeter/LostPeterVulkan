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
		uint32 m_nWidth;
		uint32 m_nHeight;
		uint32 m_nDepth;
		uint32 m_nSize;		
		uint32 m_nMipMapsCount;	
		uint32 m_nFlags;		
		FPixelFormatType m_ePixelFormat;	
		uint8 m_nPixelSize;	
		uint8* m_pBuffer;
		bool m_bAutoDelete;

	public:
		F_FORCEINLINE uint32 GetWidth() const { return m_nWidth; }
		F_FORCEINLINE uint32 GetHeight() const { return m_nHeight; }
		F_FORCEINLINE uint32 GetDepth() const { return m_nDepth; }
		F_FORCEINLINE uint32 GetSize() const { return m_nSize; }
		F_FORCEINLINE uint32 GetMipMapsCount() const { return m_nMipMapsCount; }
		F_FORCEINLINE bool HasFlag(const EImageFlagType imgFlag) const { return m_nFlags & imgFlag ? true : false; }
		F_FORCEINLINE uint32 GetFacesCount() const { return HasFlag(E_ImageFlag_IsCubeMap) ? 6 : 1;}
		F_FORCEINLINE uint32 GetRowSpan() const { return m_nWidth * m_nPixelSize; }
		F_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		F_FORCEINLINE uint32 GetPixelSize() const { return m_nPixelSize; }
		F_FORCEINLINE uint8 GetBPP() const { return m_nPixelSize * 8; }
		F_FORCEINLINE uint8* GetData() { return m_pBuffer; }
		F_FORCEINLINE const uint8* GetData() const { return m_pBuffer; }
		bool GetHasAlpha() const;
		void DestroyData();

	public:
		bool LoadDynamicImage(uint8* pData, 
                              uint32 uWidth, 
                              uint32 uHeight, 
                              uint32 uDepth, 
                              FPixelFormatType typePixelFormat, 
							  bool autoDelete = false, 
                              uint32 nFacesCount = 1, 
                              uint32 nMipMapsCount = 0);
		bool LoadDynamicImage(uint8* pData, 
                              uint32 uWidth, 
                              uint32 uHeight, 
                              FPixelFormatType typePixelFormat)
		{
			return LoadDynamicImage(pData, 
                                    uWidth, 
                                    uHeight, 
                                    1, 
                                    typePixelFormat);
		}

		bool LoadRawData(FFileMemory* pInput, 
                         uint32 uWidth, 
                         uint32 uHeight, 
                         uint32 uDepth, 
                         FPixelFormatType typePixelFormat,
						 uint32 nFacesCount = 1, 
                         uint32 nMipMapsCount = 0);
		bool LoadRawData(FFileMemory* pInput, 
                         uint32 uWidth, 
                         uint32 uHeight, 
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
		bool GetPixelBox(FPixelBox& retPB, uint32 face = 0, uint32 mipmap = 0) const;

		bool FlipAroundY();
		bool FlipAroundX();

		static bool	Scale(const FPixelBox& src, const FPixelBox& dst, EImageFilterType typeImageFilter = E_ImageFilter_Bilinear);
		bool Resize(uint16 width, uint16 height, EImageFilterType typeImageFilter = E_ImageFilter_Bilinear);

		static uint32 CalculateSize(uint32 mipmaps, uint32 faces, uint32 width, uint32 height, uint32 depth, FPixelFormatType format);
		static String GetFileExtFromMagic(FFileIO* pInput);
		static bool	ApplyGamma(uint8* buffer, float gamma, uint32 size, uint8 bpp);
	};

}; //LostPeterEngine

#endif