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

#include "../include/Image.h"
#include "../include/ImageCodec.h"
#include "../include/ImageResampler.h"

namespace LostPeterEngine
{
    Image::Image()
		: m_nWidth(0)
		, m_nHeight(0)
		, m_nDepth(0)
		, m_nSize(0)
		, m_nNumMipMaps(0)
		, m_nFlags(0)
		, m_ePixelFormat(F_PixelFormat_Unknown)
		, m_pBuffer(nullptr)
		, m_bAutoDelete(true)
	{

	}

	Image::Image(const Image& image)
		: m_pBuffer(nullptr)
		, m_bAutoDelete(true)
	{
		*this = image;
	}

	Image::~Image()
	{
		DestroyData();
	}

	Image& Image::operator =(const Image& image)
	{
		DestroyData();
		m_nWidth = image.m_nWidth;
		m_nHeight = image.m_nHeight;
		m_nDepth = image.m_nDepth;
		m_ePixelFormat = image.m_ePixelFormat;
		m_nSize	= image.m_nSize;
		m_nFlags = image.m_nFlags;
		m_nPixelSize = image.m_nPixelSize;
		m_nNumMipMaps = image.m_nNumMipMaps;
		m_bAutoDelete = image.m_bAutoDelete;
		if (m_bAutoDelete)
		{
			m_pBuffer = new uint8[m_nSize];
			memcpy(m_pBuffer, image.m_pBuffer, m_nSize);
		}
		else
		{
			m_pBuffer = image.m_pBuffer;
		}

		return *this;
	}

	void Image::DestroyData()
	{
		if (m_pBuffer && m_bAutoDelete)
		{
			F_DELETE_T(m_pBuffer)
		}
	}	

	bool Image::GetHasAlpha() const
	{
		return FPixelFormat::GetPixelFormatFlags(m_ePixelFormat) & F_PixelFormatFlag_HasAlpha;
	}

	bool Image::LoadDynamicImage(uint8* pData, 
                                 size_t uWidth, 
                                 size_t uHeight, 
                                 size_t uDepth, 
                                 FPixelFormatType typePixelFormat, 
								 bool autoDelete /*= false*/, 
                                 size_t numFaces /*= 1*/, 
                                 size_t numMipMaps /*= 0*/)
	{
		DestroyData();

		m_nWidth = uWidth;
		m_nHeight = uHeight;
		m_nDepth = uDepth;
		m_ePixelFormat = typePixelFormat;
		m_nPixelSize = static_cast<uint8>(FPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat));
		m_nNumMipMaps = numMipMaps;
		m_nFlags = 0;

		if (FPixelFormat::IsCompressed(typePixelFormat))
			m_nFlags |= E_ImageFlag_IsCompressed;
		if (m_nDepth != 1)
			m_nFlags |= E_ImageFlag_Is3DTexture;
		if (numFaces == 6)
			m_nFlags |= E_ImageFlag_IsCubeMap;
		if (numFaces != 6 && numFaces != 1)
		{
			F_LogError("Image::LoadDynamicImage: Number of faces currently must be 6 or 1 !");
			F_Assert(false && "Image::LoadDynamicImage")
			return false;
		}

		m_nSize = CalculateSize(numMipMaps, numFaces, uWidth, uHeight, uDepth, typePixelFormat);
		m_pBuffer = pData;
		m_bAutoDelete = autoDelete;
		return true;
	}

	bool Image::LoadRawData(FFileMemory* pInput, 
                            size_t uWidth, 
                            size_t uHeight, 
                            size_t uDepth, 
                            FPixelFormatType typePixelFormat,
							size_t numFaces /*= 1*/, 
                            size_t numMipMaps /*= 0*/)
	{
		size_t size = CalculateSize(numMipMaps, numFaces, uWidth, uHeight, uDepth, typePixelFormat);
		if (size != (size_t)pInput->Size())
		{
			F_LogError("Image::LoadRawData: Stream size does not match calculated image size !");
			F_Assert(false && "Image::LoadRawData")
			return false;
		}

		uint8 *buffer = new uint8[size];
		pInput->Read(buffer,size,1);

		return LoadDynamicImage(buffer, uWidth, uHeight, uDepth, typePixelFormat, true, numFaces, numMipMaps);
	}
	
	bool Image::Load(const String& strName, uint32 nGroup)
	{
		String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
		if (strPath.empty())
		{
			F_LogError("Image::Load: Image file: [%s] is not exist !", strName.c_str());
			F_Assert(false && "Image::Load: Image file is not exist!")
			return false;
		}
		return Load(strPath);
	}

	bool Image::Load(const String& strFilePath)
	{
		String strExt;
		size_t pos = strFilePath.find_last_of(".");
		if (pos != String::npos && pos < (strFilePath.length() - 1))
		{
			strExt = strFilePath.substr(pos+1);
		}
		
		FFileIO* pFIO = FFileManager::GetSingleton().CreateFileIO();
		if (!pFIO->Open(strFilePath.c_str(), "rb"))
		{
			F_LogError("Image::Load: Open file: [%s] failed !", strFilePath.c_str());
			F_Assert(false && "Image::Load")
			return false;
		}
		bool bRet = Load(pFIO,strExt);
		FFileManager::GetSingleton().DeleteFileIO(pFIO);
		return bRet;
	}

	bool Image::Load(FFileIO* pFIO, const String& strType /*= FUtilString::BLANK*/)
	{
		DestroyData();
		FCodec* pCodec = 0;
		if (!strType.empty())
		{
			pCodec = FCodecManager::GetCodec(strType);
		}
		else
		{
			size_t magicLen = FMath::Min((size_t)pFIO->Size(), (size_t)32);
			char magicBuf[32];
			pFIO->Read(magicBuf, magicLen, 1);
			pFIO->Seek(SEEK_SET, 0);
			pCodec = FCodecManager::GetCodec(magicBuf, magicLen);
		}
		if (!pCodec)
		{
			F_LogError("Image::Load: Load image: [%s] failed !", pFIO->GetFilePath().c_str());
			F_Assert(false && "Image::Load")
			return false;
		}

		//1> from file io to file memory
		long nSize = pFIO->Size();
		uint8* pBuf = new uint8[nSize];
		pFIO->Read(pBuf, nSize, 1);
		
		//2> form original file data to decode image data
		FFileMemory* pInput = FFileManager::GetSingleton().CreateFileMemory(pBuf, nSize);
		FCodec::FDecodeResult res = pCodec->Decode(pInput);
		FFileManager::GetSingleton().DeleteFileMemory(pInput);
		
		ImageCodec::ImageData* pData = static_cast<ImageCodec::ImageData*>(res.second);
		m_nWidth = pData->nWidth;
		m_nHeight = pData->nHeight;
		m_nDepth = pData->nDepth;
		m_nSize	= pData->nSize;
		m_nNumMipMaps = pData->nNumMipmaps;
		m_nFlags = pData->nFlags;
		m_ePixelFormat = pData->ePixelFormat;
		m_nPixelSize = static_cast<uint8>(FPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat));
		m_pBuffer = res.first->GetBuffer(true);
		
		FFileManager::GetSingleton().DeleteFileMemory(res.first);
		F_DELETE(pData);

		return true;
	}

	bool Image::Load(FFileMemory* pInput,const String& strType /*= FUtilString::BLANK*/)
	{
		DestroyData();
		FCodec* pCodec = 0;
		if (!strType.empty())
		{
			pCodec = FCodecManager::GetCodec(strType);
		}
		else
		{
			size_t magicLen = FMath::Min((size_t)pInput->Size(), (size_t)32);
			char magicBuf[32];
			pInput->Read(magicBuf, magicLen, 1);
			pInput->Seek(SEEK_SET,0);
			pCodec = FCodecManager::GetCodec(magicBuf,magicLen);
		}
		if (!pCodec)
		{
			F_LogError("Image::Load: Load image from memory failed !");
			F_Assert(false && "Image::Load")
			return false;
		}

		//1> form original memory data to decode image data
		FCodec::FDecodeResult res = pCodec->Decode(pInput);
		ImageCodec::ImageData* pImageData = static_cast<ImageCodec::ImageData*>(res.second);
		m_nWidth = pImageData->nWidth;
		m_nHeight = pImageData->nHeight;
		m_nDepth = pImageData->nDepth;
		m_nSize	= pImageData->nSize;
		m_nNumMipMaps = pImageData->nNumMipmaps;
		m_nFlags = pImageData->nFlags;
		m_ePixelFormat = pImageData->ePixelFormat;
		m_nPixelSize = static_cast<uint8>(FPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat));
		m_pBuffer = res.first->GetBuffer(true);

		FFileManager::GetSingleton().DeleteFileMemory(res.first);
		F_DELETE(pImageData);

		return true;
	}

	bool Image::Save(const String& strFilePath)
	{
		if(!m_pBuffer)
		{
			F_LogError("Image::Save: No image data loaded !");
			F_Assert(false && "Image::Save")
			return false;
		}

		String strExt;
		size_t pos = strFilePath.find_last_of(".");
		if (pos == String::npos)
		{
			F_LogError("Image::Save: Unable to save image file: [%s] - invalid extension !", strFilePath.c_str());
			F_Assert(false && "Image::Save")
			return false;
		}
		while (pos != strFilePath.length() - 1)
		{
            strExt += strFilePath[++pos];
        }

		FCodec* pCodec = FCodecManager::GetCodec(strExt);
		if (!pCodec)
		{
			F_LogError("Image::Save: Unable to save image file: [%s] - invalid extension !", strFilePath.c_str());
			F_Assert(false && "Image::Save")
			return false;
		}
		
		FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory(m_pBuffer, (uint32)m_nSize);
		pFM->SetFreeOnClose(false);

		ImageCodec::ImageData* pImageData = new ImageCodec::ImageData();
		pImageData->nHeight = (int32)m_nHeight;
		pImageData->nWidth = (int32)m_nWidth;
		pImageData->nDepth = (int32)m_nDepth;
		pImageData->ePixelFormat = m_ePixelFormat;

		bool bRet = pCodec->CodeToFile(pFM, strFilePath, pImageData);
		if (!bRet)
		{
			F_LogError("Image::Save: Unable to code image file: [%s] !", strFilePath.c_str());
			F_Assert(false && "Image::Save")
			return false;
		}
		
		FFileManager::GetSingleton().DeleteFileMemory(pFM);
		F_DELETE(pImageData)
		return bRet;
	}

	FFileMemory* Image::Encode(const String& strExt)
	{
		if (!m_pBuffer)
		{
			F_LogError("Image::Encode: No image data loaded !");
			F_Assert(false && "Image::Encode")
			return nullptr;
		}

		FCodec* pCodec = FCodecManager::GetCodec(strExt);
		if (!pCodec)
		{
			F_LogError("Image::Encode: Unable to encode image data as: [%s] - invalid extension !", strExt.c_str());
			F_Assert(false && "Image::Encode")
			return nullptr;
		}

		FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory(m_pBuffer, (uint32)m_nSize);
		pFM->SetFreeOnClose(false);

		ImageCodec::ImageData* pImageData = new ImageCodec::ImageData();
		pImageData->nHeight = (int32)m_nHeight;
		pImageData->nWidth = (int32)m_nWidth;
		pImageData->nDepth = (int32)m_nDepth;
		pImageData->ePixelFormat = m_ePixelFormat;

		FFileMemory* pRet = pCodec->Code(pFM, pImageData);
		FFileManager::GetSingleton().DeleteFileMemory(pFM);
		F_DELETE(pImageData)
		return pRet;
	}

	FColor Image::GetColorAt(int x, int y, int z /*= -1*/) const
	{
		FColor rval;
		FPixelFormat::UnpackColor(&rval, m_ePixelFormat, &m_pBuffer[m_nPixelSize * (z * m_nWidth * m_nHeight + m_nWidth * y + x)]);
		return rval;
	}

	bool Image::GetPixelBox(FPixelBox& retPB, size_t face /*= 0*/, size_t mipmap /*= 0*/) const
	{
		// Image data is arranged as:
		// face 0, top level (mip 0)
		// face 0, mip 1
		// face 0, mip 2
		// face 1, top level (mip 0)
		// face 1, mip 1
		// face 1, mip 2
		// etc
		if (mipmap > GetNumMipMaps())
		{
			F_LogError("Image::GetPixelBox: Mipmap index out of range !");
			F_Assert(false && "Image::GetPixelBox")
			return false;
		}

		if (face >= GetNumFaces())
		{
			F_LogError("Image::GetPixelBox: Face index out of range !");
			F_Assert(false && "Image::GetPixelBox")
			return false;
		}

		uint8* pOffset = const_cast<uint8*>(GetData());
		size_t width = GetWidth();
        size_t height = GetHeight();
        size_t depth = GetDepth();
		size_t numMips = GetNumMipMaps();

		// Figure out the offsets 
		size_t fullFaceSize  = 0;
		size_t finalFaceSize = 0;
		size_t finalWidth	 = 0; 
		size_t finalHeight   = 0;
		size_t finalDepth	 = 0;
		for (size_t mip = 0; mip <= numMips; ++mip)
		{
			if (mip == mipmap)
			{
				finalFaceSize = fullFaceSize;
				finalWidth = width;
				finalHeight = height;
				finalDepth = depth;
			}
			fullFaceSize += FPixelFormat::GetPixelFormatMemorySize(width, height, depth, GetPixelFormat());

			if(width != 1) 
				width /= 2;
			if(height != 1) 
				height /= 2;
			if(depth != 1) 
				depth /= 2;
		}
		pOffset += face * fullFaceSize;
		pOffset += finalFaceSize;
		
		retPB = FPixelBox(finalWidth, finalHeight, finalDepth, GetPixelFormat(), pOffset);
		return true;
	}

	bool Image::FlipAroundY()
	{
		if (!m_pBuffer)
		{
			F_LogError("Image::FlipAroundY: Can not flip an unintialized texture !");
			F_Assert(false && "Image::FlipAroundY")
			return false;
		}

		m_nNumMipMaps = 0; 
		uint8* pTempBuffer1 = 0;
		uint16* pTempBuffer2 = 0;
		uint8* pTempBuffer3 = 0;
		unsigned int* pTempBuffer4 = 0;
		uint8* src1 = m_pBuffer, *dst1 = 0;
		uint16* src2 = (uint16 *)m_pBuffer, *dst2 = 0;
		uint8* src3 = m_pBuffer, *dst3 = 0;
		unsigned int* src4 = (unsigned int *)m_pBuffer, *dst4 = 0;

		uint16 y;
		switch (m_nPixelSize)
		{
		case 1:
			{
				pTempBuffer1 = new uint8[m_nWidth * m_nHeight];
				for (y = 0; y < m_nHeight; y++)
				{
					dst1 = (pTempBuffer1 + ((y * m_nWidth) + m_nWidth - 1));
					for (uint16 x = 0; x < m_nWidth; x++)
					{
                        memcpy(dst1--, src1++, sizeof(uint8));
                    }
				}
				memcpy(m_pBuffer, pTempBuffer1, m_nWidth * m_nHeight * sizeof(uint8));
                F_DELETE_T(pTempBuffer1)
			}
			break;

		case 2:
			{
				pTempBuffer2 = new uint16[m_nWidth * m_nHeight];
				for (y = 0; y < m_nHeight; y++)
				{
					dst2 = (pTempBuffer2 + ((y * m_nWidth) + m_nWidth - 1));
					for (uint16 x = 0; x < m_nWidth; x++)
					{
                        memcpy(dst2--, src2++, sizeof(uint16));
                    }
				}
				memcpy(m_pBuffer, pTempBuffer2, m_nWidth * m_nHeight * sizeof(uint16));
                F_DELETE_T(pTempBuffer2)
			}
			break;

		case 3:
			{
				pTempBuffer3 = new uint8[m_nWidth * m_nHeight * 3];
				for (y = 0; y < m_nHeight; y++)
				{
					size_t offset = ((y * m_nWidth) + (m_nWidth - 1)) * 3;
					dst3 = pTempBuffer3;
					dst3 += offset;
					for (size_t x = 0; x < m_nWidth; x++)
					{
						memcpy(dst3, src3, sizeof(uint8) * 3);
						dst3 -= 3; src3 += 3;
					}
				}
				memcpy(m_pBuffer, pTempBuffer3, m_nWidth * m_nHeight * sizeof(uint8) * 3);
                F_DELETE_T(pTempBuffer3)
			}
			break;

		case 4:
			{
				pTempBuffer4 = new unsigned int[m_nWidth * m_nHeight];
				for (y = 0; y < m_nHeight; y++)
				{
					dst4 = (pTempBuffer4 + ((y * m_nWidth) + m_nWidth - 1));
					for (uint16 x = 0; x < m_nWidth; x++)
					{
                        memcpy(dst4--, src4++, sizeof(unsigned int));
                    }
				}
				memcpy(m_pBuffer, pTempBuffer4, m_nWidth * m_nHeight * sizeof(unsigned int));
				F_DELETE_T(pTempBuffer4)
			}
			break;

		default:
			{
				F_LogError("Image::FlipAroundY: Unknown pixel depth !");
				F_Assert(false && "Image::FlipAroundY")
				return false;
			}
			break;
		}

		return true;
	}

	bool Image::FlipAroundX()
	{
		if (!m_pBuffer)
		{
			F_LogError("Image::FlipAroundX: Can not flip an unitialized texture !");
			F_Assert(false && "Image::FlipAroundX")
			return false;
		}

		m_nNumMipMaps = 0;
		size_t rowSpan = m_nWidth * m_nPixelSize;
		uint8 *pTempBuffer = new uint8[rowSpan * m_nHeight];
		uint8 *ptr1 = m_pBuffer, *ptr2 = pTempBuffer + ((m_nHeight - 1) * rowSpan);

		for(uint16 i = 0; i < m_nHeight; i++)
		{
			memcpy( ptr2, ptr1, rowSpan );
			ptr1 += rowSpan; ptr2 -= rowSpan;
		}
		memcpy(m_pBuffer, pTempBuffer, rowSpan * m_nHeight);
		F_DELETE_T(pTempBuffer)

		return true;
	}

	bool Image::Scale(const FPixelBox& src, const FPixelBox& scaled, EImageFilterType typeImageFilter /*= E_ImageFilter_Bilinear*/)
	{
		F_Assert(FPixelFormat::IsAccessible(src.m_ePixelFormat) && "Image::Scale")
		F_Assert(FPixelFormat::IsAccessible(scaled.m_ePixelFormat) && "Image::Scale")
		uint8* pData = 0;
		FPixelBox temp;
		switch ((int32)typeImageFilter) 
		{
		case E_ImageFilter_Nearest:
			{
				if (src.m_ePixelFormat == scaled.m_ePixelFormat) 
				{
					temp = scaled;
				}
				else
				{
					temp = FPixelBox(scaled.GetWidth(), scaled.GetHeight(), scaled.GetDepth(), src.m_ePixelFormat);
					pData = new uint8[temp.GetConsecutiveSize()];
					temp.m_pData = pData;
				}
				// super-optimized: no conversion
				switch (FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat)) 
				{
				case 1:  ImageNearestResampler<1>::Scale(src, temp); break;
				case 2:  ImageNearestResampler<2>::Scale(src, temp); break;
				case 3:  ImageNearestResampler<3>::Scale(src, temp); break;
				case 4:  ImageNearestResampler<4>::Scale(src, temp); break;
				case 6:  ImageNearestResampler<6>::Scale(src, temp); break;
				case 8:  ImageNearestResampler<8>::Scale(src, temp); break;
				case 12: ImageNearestResampler<12>::Scale(src, temp); break;
				case 16: ImageNearestResampler<16>::Scale(src, temp); break;
				default:
					F_Assert(false && "Image::Scale")
				}
				if(temp.m_pData != scaled.m_pData)
				{
					FPixelFormat::BulkPixelConversion(temp,scaled);
				}
			}
			break;

		case E_ImageFilter_Linear:
		case E_ImageFilter_Bilinear:
			{
				switch (src.m_ePixelFormat) 
				{
				case F_PixelFormat_BYTE_L8_UNORM: 
				case F_PixelFormat_BYTE_A8_UNORM: 
				case F_PixelFormat_BYTE_AL_UNORM:
                case F_PixelFormat_BYTE_R8G8B8_SRGB: 
				case F_PixelFormat_BYTE_B8G8R8_SRGB:
				case F_PixelFormat_BYTE_R8G8B8_UNORM: 
				case F_PixelFormat_BYTE_B8G8R8_UNORM:
                case F_PixelFormat_BYTE_A8R8G8B8_SRGB:
				case F_PixelFormat_BYTE_A8B8G8R8_SRGB:
				case F_PixelFormat_BYTE_A8R8G8B8_UNORM: 
				case F_PixelFormat_BYTE_A8B8G8R8_UNORM:
					{
						if(src.m_ePixelFormat == scaled.m_ePixelFormat) 
						{
							temp = scaled;
						}
						else
						{
							temp = FPixelBox(scaled.GetWidth(), scaled.GetHeight(), scaled.GetDepth(), src.m_ePixelFormat);
							pData = new uint8[temp.GetConsecutiveSize()]; 
							temp.m_pData = pData;
						}
						switch (FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat)) 
						{
						case 1: ImageLinearResampler_Byte<1>::Scale(src, temp); break;
						case 2: ImageLinearResampler_Byte<2>::Scale(src, temp); break;
						case 3: ImageLinearResampler_Byte<3>::Scale(src, temp); break;
						case 4: ImageLinearResampler_Byte<4>::Scale(src, temp); break;
						default:
							F_Assert(false && "Image::Scale")
						}
						if(temp.m_pData != scaled.m_pData)
						{
							FPixelFormat::BulkPixelConversion(temp, scaled);
						}
					}
					break;
				case F_PixelFormat_INT_B32G32R32_SFLOAT:
				case F_PixelFormat_INT_A32B32G32R32_SFLOAT:
					if (scaled.m_ePixelFormat == F_PixelFormat_INT_B32G32R32_SFLOAT || 
						scaled.m_ePixelFormat == F_PixelFormat_INT_A32B32G32R32_SFLOAT)
					{
						ImageLinearResampler_Float32::Scale(src, scaled);
						break;
					}
				default:
					ImageLinearResampler_Float32::Scale(src, scaled);
				}
			}
			break;
		}
		
		F_DELETE_T(pData)
		return true;
	}

	bool Image::Resize(uint16 width, uint16 height, EImageFilterType typeImageFilter /*= E_ImageFilter_Bilinear*/)
	{
		F_Assert(m_bAutoDelete && "Image::Scale")
		F_Assert(m_nDepth == 1 && "Image::Scale")
		
		Image temp;
		temp.LoadDynamicImage(m_pBuffer, m_nWidth, m_nHeight, 1, m_ePixelFormat, true);
		
		m_nWidth = width;
		m_nHeight = height;
		m_nSize	= FPixelFormat::GetPixelFormatMemorySize(m_nWidth, m_nHeight, 1, m_ePixelFormat);
		m_pBuffer = new uint8[m_nSize];
		m_nNumMipMaps = 0;
		
		FPixelBox src;
		temp.GetPixelBox(src);
		FPixelBox dst;
		GetPixelBox(dst);
		return Image::Scale(src,dst, typeImageFilter);
	}

	size_t Image::CalculateSize(size_t mipmaps, size_t faces, size_t width, size_t height, size_t depth, FPixelFormatType format)
	{
		size_t size = 0;
		for(size_t mip = 0; mip <= mipmaps; ++mip)
		{
			size += FPixelFormat::GetPixelFormatMemorySize(width, height, depth, format) * faces; 
			if (width != 1)  width /= 2;
			if (height != 1) height /= 2;
			if (depth != 1)  depth /= 2;
		}
		return size;
	}

	String Image::GetFileExtFromMagic(FFileIO* pInput)
	{
		size_t magicLen = FMath::Min((size_t)pInput->Size(), (size_t)32);
		char magicBuf[32];
		pInput->Read(magicBuf, magicLen, 1);
		pInput->Seek(SEEK_SET, 0);
		FCodec* pCodec = FCodecManager::GetCodec(magicBuf, magicLen);

		if(pCodec)
			return pCodec->GetCodecType();
		
		return FUtilString::BLANK;
	}

	bool Image::ApplyGamma(uint8* buffer, float gamma, size_t size, uint8 bpp)
	{
		if(gamma == 1.0f)
			return false;

		//NB only 24/32-bit supported
		if (bpp != 24 && bpp != 32) 
			return false;

		unsigned int stride = bpp >> 3;

		for (size_t i = 0, j = size / stride; i < j; i++, buffer += stride)
		{
			float r, g, b;

			r = (float)buffer[0];
			g = (float)buffer[1];
			b = (float)buffer[2];

			r = r * gamma;
			g = g * gamma;
			b = b * gamma;

			float scale = 1.0f, tmp;

			if(r > 255.0f && (tmp=(255.0f/r)) < scale)
				scale = tmp;
			if(g > 255.0f && (tmp=(255.0f/g)) < scale)
				scale = tmp;
			if(b > 255.0f && (tmp=(255.0f/b)) < scale)
				scale = tmp;

			r *= scale; g *= scale; b *= scale;

			buffer[0] = (uint8)r;
			buffer[1] = (uint8)g;
			buffer[2] = (uint8)b;
		}

		return true;
	}

}; //LostPeterEngine