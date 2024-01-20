/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ImageCodecDXT.h"
#include "../include/Image.h"
#include "squish.h"

namespace LostPeterEngine
{
#if F_PLATFORM == F_PLATFORM_WINDOW
	#pragma warning (disable : 4996)
#endif

#if F_COMPILER == F_COMPILER_MSVC
	#pragma pack (push, 1)
#else
	#pragma pack (1)
#endif

    struct DDSPixelFormat
	{
		uint32 size;
		uint32 flags;
		uint32 fourCC;
		uint32 rgbBits;
		uint32 redMask;
		uint32 greenMask;
		uint32 blueMask;
		uint32 alphaMask;
	};

	struct DDSCaps
	{
		uint32 caps1;
		uint32 caps2;
		uint32 reserved[2];
	};

	struct DDSHeader
	{
		uint32 size;		
		uint32 flags;
		uint32 height;
		uint32 width;
		uint32 sizeOrPitch;
		uint32 depth;
		uint32 mipMapCount;
		uint32 reserved1[11];
		DDSPixelFormat pixelFormat;
		DDSCaps caps;
		uint32 reserved2;
	};

	// An 8-byte DXT color block, represents a 4x4 texel area. Used by all DXT formats
	struct DXTColorBlock
	{
		// 2 color ranges
		uint16 color_0;
		uint16 color_1;
		// 16 2-bit indexes, each byte here is one row
		uint8 indexRow[4];
	};
	// An 8-byte DXT explicit alpha block, represents a 4x4 texel area. Used by DXT2/3
	struct DXTExplicitAlphaBlock
	{
		// 16 4-bit values, each 16-bit value is one row
		uint16 alphaRow[4];
	};
	// An 8-byte DXT interpolated alpha block, represents a 4x4 texel area. Used by DXT4/5
	struct DXTInterpolatedAlphaBlock
	{
		// 2 alpha ranges
		uint8 alpha_0;
		uint8 alpha_1;
		// 16 3-bit indexes. Unfortunately 3 bits doesn't map too well to row bytes
		// so just stored raw
		uint8 indexes[6];
	};

#if F_COMPILER == F_COMPILER_MSVC
	#pragma pack (pop)
#else
	#pragma pack ()
#endif

    const uint32 DDS_PIXELFORMAT_SIZE	= 8 * sizeof(uint32);
	const uint32 DDS_CAPS_SIZE			= 4 * sizeof(uint32);
	const uint32 DDS_HEADER_SIZE		= 19 * sizeof(uint32) + DDS_PIXELFORMAT_SIZE + DDS_CAPS_SIZE;

	const uint32 DDSD_CAPS				= 0x00000001;
	const uint32 DDSD_HEIGHT			= 0x00000002;
	const uint32 DDSD_WIDTH				= 0x00000004;
	const uint32 DDSD_PITCH				= 0x00000008;
	const uint32 DDSD_PIXELFORMAT 		= 0x00001000;
	const uint32 DDSD_MIPMAPCOUNT 		= 0x00020000;
	const uint32 DDSD_LINEARSIZE		= 0x00080000;
	const uint32 DDSD_DEPTH				= 0x00800000;
	const uint32 DDPF_ALPHAPIXELS		= 0x00000001;
	const uint32 DDPF_FOURCC			= 0x00000004;
	const uint32 DDPF_RGB				= 0x00000040;
	const uint32 DDSCAPS_COMPLEX		= 0x00000008;
	const uint32 DDSCAPS_TEXTURE		= 0x00001000;
	const uint32 DDSCAPS_MIPMAP			= 0x00400000;
	const uint32 DDSCAPS2_CUBEMAP		= 0x00000200;
	const uint32 DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400;
	const uint32 DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800;
	const uint32 DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000;
	const uint32 DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000;
	const uint32 DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000;
	const uint32 DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000;
	const uint32 DDSCAPS2_VOLUME		= 0x00200000;

	// Special FourCC codes
	const uint32 D3DFMT_R16F			= 111;
	const uint32 D3DFMT_G16R16F			= 112;
	const uint32 D3DFMT_A16B16G16R16F	= 113;
	const uint32 D3DFMT_R32F            = 114;
	const uint32 D3DFMT_G32R32F         = 115;
	const uint32 D3DFMT_A32B32G32R32F   = 116;

    
	class ImageCodecDXT* ImageCodecDXT::ms_pInstance = nullptr;
    ImageCodecDXT::ImageCodecDXT()
        : m_strType("dxt")
    {

    }

    ImageCodecDXT::~ImageCodecDXT()
    {
        
    }

    void ImageCodecDXT::Startup()
    {
        if (!ms_pInstance)
		{
			F_LogInfo("ImageCodecDXT::Startup: DXT codec registering !");
			ms_pInstance = new ImageCodecDXT();
			FCodecManager::RegisterCodec(ms_pInstance);
		}
    }

	void ImageCodecDXT::Shutdown()
    {
        if (ms_pInstance)
		{
			FCodecManager::UnRegisterCodec(ms_pInstance);
			F_DELETE(ms_pInstance)
			F_LogInfo("ImageCodecDXT::Shutdown: DXT codec unregistering !");
		}
    }

    String ImageCodecDXT::GetCodecType() const 
    { 
        return m_strType; 
    }

	String ImageCodecDXT::MagicNumberToFileExt(const char* szMagicNum, size_t nMaxbytes) const
    {
        return ".dds";
    }

    FFileMemory* ImageCodecDXT::Code(FFileMemory* pInput, FCodecData* pCodecData) const
    {
        ImageData* pImgData = static_cast<ImageData*>(pCodecData);

		FPixelFormatType ePixelFormatDXT = F_PixelFormat_Unknown;
		switch ((int32)pImgData->ePixelFormat)
		{
		case F_PixelFormat_BYTE_R8G8B8_SRGB:
        case F_PixelFormat_BYTE_B8G8R8_SRGB:
			ePixelFormatDXT = F_PixelFormat_DXT1;
			break;
		case F_PixelFormat_BYTE_A8R8G8B8_SRGB:
		case F_PixelFormat_BYTE_A8B8G8R8_SRGB:
			ePixelFormatDXT = F_PixelFormat_DXT5;
			break;
		}

		if (F_PixelFormat_Unknown == ePixelFormatDXT)
		{
			F_LogError("*********************** ImageCodecDXT::Code: DXT encoding not supported !");
			F_Assert(false && "ImageCodecDXT::Code: DXT encoding not supported !")
			return nullptr;
		}

		uint32 numFaces = 1;
		if(pImgData->nFlags & E_ImageFlag_IsCubeMap)
		{
			numFaces = 6;
		}
		uint32 nPixelSize = Image::CalculateSize((uint32)pImgData->nMipmapsCount, 
                                                 numFaces,
			                                     (uint32)pImgData->nWidth, 
                                                 (uint32)pImgData->nHeight, 
                                                 (uint32)pImgData->nDepth, 
                                                 ePixelFormatDXT);

		uint32 nAllSize = sizeof(uint32) + sizeof(DDSHeader) + nPixelSize;
		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory(nAllSize);
		uint8* pImageHead = pOut->GetBuffer();

		uint32* pFileType = (uint32*)(pImageHead);
		*pFileType = F_M_FOURCC('D', 'D', 'S', ' ');
		ImageCodec::FlipEndian(pFileType, sizeof(uint32), 1);

		DDSHeader* pHeader = (DDSHeader*)(pFileType + 1);
		memset(pHeader, 0, sizeof(DDSHeader));
		pHeader->size = DDS_HEADER_SIZE;
		pHeader->pixelFormat.size = DDS_PIXELFORMAT_SIZE;

		pHeader->width = pImgData->nWidth;
		pHeader->height = pImgData->nHeight;
		pHeader->flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE;

		if (pImgData->nDepth > 1)
		{
			pHeader->flags = (pHeader->flags)|DDSD_DEPTH;
			pHeader->depth = pImgData->nDepth;
		}
		pHeader->mipMapCount		= pImgData->nMipmapsCount + 1;
		pHeader->pixelFormat.fourCC = convertFormatToFourCC(ePixelFormatDXT);		
		pHeader->pixelFormat.flags	= DDPF_FOURCC;
		pHeader->sizeOrPitch		= pHeader->width * pHeader->height;
		pHeader->caps.caps1			= DDSCAPS_COMPLEX | DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		if(pImgData->nFlags & E_ImageFlag_IsCubeMap)
		{
			pHeader->caps.caps2 |= DDSCAPS2_CUBEMAP;
		}
		else if (pImgData->nFlags &  E_ImageFlag_Is3DTexture)
		{
			pHeader->caps.caps2 |= DDSCAPS2_VOLUME;		
		}
		ImageCodec::FlipEndian(pHeader, 4, sizeof(DDSHeader) / 4);

		uint8* srcData = pInput->GetBuffer();
		uint8* desData = (uint8*)(pHeader + 1);

		for (size_t i = 0; i < numFaces; ++i)
		{
			size_t width  = pImgData->nWidth;
			size_t height = pImgData->nHeight;
			size_t depth  = pImgData->nDepth;
			for (size_t mip = 0; mip <= pImgData->nMipmapsCount; ++mip)
			{
				size_t SrcBpp			= FPixelFormat::GetPixelFormatElemBytes(pImgData->ePixelFormat);
				size_t SrcPitch			= width * SrcBpp;
				size_t SrcPitchMinus4	= SrcPitch - SrcBpp * 4;

				for (size_t z = 0; z < depth; ++z)
				{
					// 4x4 blocks in x/y
					for (size_t y = 0; y < height; y += 4)
					{
						for (size_t x = 0; x < width; x += 4)
						{
							squish::u8 block[16];
							RGBA Input [16];
							for (size_t by = 0; by < 4; ++by)
							{
								for (size_t bx = 0; bx < 4; ++bx)
								{
									uint8* pWiseBit =  (uint8*)&Input[by*4+bx];
									FPixelFormat::UnpackColor(&pWiseBit[0], 
                                                              &pWiseBit[1], 
                                                              &pWiseBit[2], 
                                                              &pWiseBit[3],
															  pImgData->ePixelFormat, 
                                                              srcData);
									srcData = static_cast<uint8*>(srcData) + SrcBpp;
								}
								srcData =  static_cast<uint8*>(srcData) + SrcPitchMinus4;
							}
							squish::Compress((squish::u8*)Input, block, _DXTColorTypeToSquishType(ePixelFormatDXT));
							
							DXTColorBlock* pColor = (DXTColorBlock*)block;
							DXTExplicitAlphaBlock* pEAlpha = 0;
							DXTInterpolatedAlphaBlock* pIAlpha = 0;

							if (/*sourceFormat == F_PixelFormat_DXT2 ||*/ 
								ePixelFormatDXT == F_PixelFormat_DXT3)
							{
								// explicit alpha
								pEAlpha = (DXTExplicitAlphaBlock*)pColor;
								pColor++;
								ImageCodec::FlipEndian(pEAlpha->alphaRow, sizeof(uint16), 4);
								memcpy(desData, pEAlpha, sizeof(DXTExplicitAlphaBlock));
								desData += sizeof(DXTExplicitAlphaBlock);
							}
							else if (/*sourceFormat == F_PixelFormat_DXT4 || */
									 ePixelFormatDXT == F_PixelFormat_DXT5)
							{
								// interpolated alpha
								pIAlpha = (DXTInterpolatedAlphaBlock*)pColor;
								pColor++;
								ImageCodec::FlipEndian(&(pIAlpha->alpha_0), sizeof(uint16), 1);
								ImageCodec::FlipEndian(&(pIAlpha->alpha_1), sizeof(uint16), 1);
								memcpy(desData, pIAlpha, sizeof(DXTInterpolatedAlphaBlock));
								desData += sizeof(DXTInterpolatedAlphaBlock);
							}

							ImageCodec::FlipEndian(&(pColor->color_0), sizeof(uint16), 1);
							ImageCodec::FlipEndian(&(pColor->color_1), sizeof(uint16), 1);
							memcpy(desData, pColor, sizeof(DXTColorBlock));
							desData += sizeof(DXTColorBlock);

							// next block. Our dest pointer is 4 lines down
							// from where it started
							if (x + 4 == width)
							{
								// Jump back to the start of the line
								srcData = static_cast<uint8*>(srcData) - SrcPitchMinus4;
							}
							else
							{
								// Jump back up 4 rows and 4 pixels to the
								// right to be at the next block to the right
								srcData = static_cast<uint8*>(srcData) - SrcPitch * 4 + SrcBpp * 4;

							}
						}
					}
				}

				if (width!=1)    width /= 2;
				if (height!=1)   height /= 2;
				if (depth!=1)    depth /= 2;
			}
		}

		return pOut;
    }

	bool ImageCodecDXT::CodeToFile(FFileMemory* pInput, const String& outFilePath, FCodecData* pCodecData) const
    {
        FILE* fp = fopen(outFilePath.c_str(), "wb");
		if (!fp)
        {
            F_LogError("*********************** ImageCodecDXT::CodeToFile: Can not open file: [%s] !", outFilePath.c_str());
			F_Assert(false && "ImageCodecDXT::CodeToFile: Can not open file !")
            return false;
        }
		FFileMemory* pOut = Code(pInput, pCodecData);
		if (!pOut)
        {
            F_LogError("*********************** ImageCodecDXT::CodeToFile: Code file: [%s] failed !", outFilePath.c_str());
			F_Assert(false && "ImageCodecDXT::CodeToFile: Code file failed !")
            return false;
        }
		
		fwrite(pOut->GetBuffer(), pOut->Size(), 1, fp);
		fclose(fp);
		FFileManager::GetSingleton().DeleteFileMemory(pOut);

		return true;
    }

	ImageCodecDXT::FDecodeResult ImageCodecDXT::Decode(FFileMemory* pInput) const
    {
        // Read 4 character code
		uint32 fileType;
		pInput->Read(&fileType, sizeof(uint32), 1);
		ImageCodec::FlipEndian(&fileType, sizeof(uint32), 1);

		if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType)
		{
			F_LogError("*********************** ImageCodecDXT::Decode: This is not a DDS file !");
			F_Assert(false && "ImageCodecDXT::Decode: This is not a DDS file !")
			return FDecodeResult();
		}

        // Read header in full
		DDSHeader header;
		pInput->Read(&header, sizeof(DDSHeader), 1);

		// Endian flip if required, all 32-bit values
		ImageCodec::FlipEndian(&header, 4, sizeof(DDSHeader) / 4);

		// Check some sizes
		if (header.size != DDS_HEADER_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::Decode: DDS header size mismatch !");
			F_Assert(false && "ImageCodecDXT::Decode: DDS header size mismatch !")
			return FDecodeResult();
		}
		if (header.pixelFormat.size != DDS_PIXELFORMAT_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::Decode: DDS header pixel size mismatch !");
			F_Assert(false && "ImageCodecDXT::Decode: DDS header pixel size mismatch !")
			return FDecodeResult();
		}

        ImageData* pImageData = new ImageData();
		pImageData->nDepth	= 1; // (deal with volume later)
		pImageData->nWidth	= header.width;
		pImageData->nHeight = header.height;
		uint32 numFaces	= 1; // assume one face until we know otherwise
		if (header.caps.caps1 & DDSCAPS_MIPMAP)
		{
			pImageData->nMipmapsCount = uint16(header.mipMapCount - 1);
		}
		else
		{
			pImageData->nMipmapsCount = 0;
		}
		pImageData->nFlags = 0;

		size_t width = pImageData->nWidth;
		size_t height = pImageData->nHeight;
		size_t iTemp = width < height ? width : height;
		uint32 mipMapMaxCount = 0;
		while (iTemp >= 8)
		{
			iTemp /= 2;
			mipMapMaxCount++;
		}
		pImageData->nMipmapsCount = uint16(pImageData->nMipmapsCount < mipMapMaxCount ? pImageData->nMipmapsCount : mipMapMaxCount);


		bool decompressDXT = false;
		// Figure out basic image type
		if (header.caps.caps2 & DDSCAPS2_CUBEMAP)
		{
			pImageData->nFlags |= E_ImageFlag_IsCubeMap;
			numFaces = 6;
		}
		else if (header.caps.caps2 & DDSCAPS2_VOLUME)
		{
			pImageData->nFlags |= E_ImageFlag_Is3DTexture;
			pImageData->nDepth  = header.depth;
		}
		// Pixel format
		FPixelFormatType sourceFormat = F_PixelFormat_Unknown;

		if (header.pixelFormat.flags & DDPF_FOURCC)
		{

			sourceFormat = convertFourCCFormat(header.pixelFormat.fourCC);
		}
		else
		{
			sourceFormat = convertPixelFormat(header.pixelFormat.rgbBits, 
											  header.pixelFormat.redMask, 
											  header.pixelFormat.greenMask, 
										 	  header.pixelFormat.blueMask, 
											  header.pixelFormat.flags & DDPF_ALPHAPIXELS ? 
											  header.pixelFormat.alphaMask : 0);
		}

		bool bhasCapability = false;
		if (FPixelFormat::IsCompressed(sourceFormat))
		{
			bhasCapability = false;
			if (!bhasCapability)
			{
				decompressDXT = true;
				switch ((int32)sourceFormat)
				{
				case F_PixelFormat_DXT1:
					// source can be either 565 or 5551 depending on whether alpha present
					// unfortunately you have to read a block to figure out which
					// Note that we upgrade to 32-bit pixel formats here, even 
					// though the source is 16-bit; this is because the interpolated
					// values will benefit from the 32-bit results, and the source
					// from which the 16-bit samples are calculated may have been
					// 32-bit so can benefit from this.
					DXTColorBlock block;
					pInput->Read(&block, sizeof(DXTColorBlock), 1);
					ImageCodec::FlipEndian(&(block.color_0),sizeof(uint16),1);
					ImageCodec::FlipEndian(&(block.color_1),sizeof(uint16),1);
					// skip back since we'll need to read this again
					pInput->Seek(long(0 - sizeof(DXTColorBlock)), SEEK_CUR);
					pImageData->ePixelFormat = F_PixelFormat_BYTE_R8G8B8_SRGB;
					//}
					break;
				//case F_PixelFormat_DXT2:
				case F_PixelFormat_DXT3:
				//case F_PixelFormat_DXT4:
				case F_PixelFormat_DXT5:
					pImageData->ePixelFormat = F_PixelFormat_BYTE_A8R8G8B8_SRGB;
					break;
				default:
					// all other cases need no special format handling
					break;
				}
			}
			else
			{
				// Use original format
				pImageData->ePixelFormat = sourceFormat;
				// Keep DXT data compressed
				pImageData->nFlags |= E_ImageFlag_IsCompressed;
			}
		}
		else // not compressed
		{
			// Don't test against DDPF_RGB since greyscale DDS doesn't set this
			// just derive any other kind of format
			pImageData->ePixelFormat = sourceFormat;
		}

		// Calculate total size from number of mipmaps, faces and size
		pImageData->nSize = (int32)Image::CalculateSize((uint32)pImageData->nMipmapsCount, 
                                                        numFaces, 
														(uint32)pImageData->nWidth, 
														(uint32)pImageData->nHeight, 
														(uint32)pImageData->nDepth, 
														pImageData->ePixelFormat);

		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory((uint32)pImageData->nSize);
		// Now deal with the data
		void* destPtr = pOut->GetBuffer();

		// all mips for a face, then each face
		for (uint32 i = 0; i < numFaces; ++i)
		{   
			size_t width  = pImageData->nWidth;
			size_t height = pImageData->nHeight;
			size_t depth  = pImageData->nDepth;

			for(size_t mip = 0; mip <= pImageData->nMipmapsCount; ++mip)
			{
				size_t dstPitch = width * FPixelFormat::GetPixelFormatElemBytes(pImageData->ePixelFormat);
				if (FPixelFormat::IsCompressed(sourceFormat))
				{
					// Compressed data
					//decompressDXT = true;
					if (decompressDXT)
					{
						/*
						DXTColorBlock col;
						DXTInterpolatedAlphaBlock iAlpha;
						DXTExplicitAlphaBlock eAlpha;
						*/
						// 4x4 block of decompressed color
						//FColor tempColors[16];
						size_t destBpp = FPixelFormat::GetPixelFormatElemBytes(pImageData->ePixelFormat);
						size_t destPitchMinus4 = dstPitch - destBpp * 4;
						// slices are done individually
						for(size_t z = 0; z < depth; ++z)
						{
							// 4x4 blocks in x/y
							for (size_t y = 0; y < height; y += 4)
							{
								for (size_t x = 0; x < width; x += 4)
								{
									squish::u8 block[16];
									DXTColorBlock* pColor = (DXTColorBlock*)block;
									DXTExplicitAlphaBlock* pEAlpha = 0;
									DXTInterpolatedAlphaBlock* pIAlpha = 0;

									if (/*sourceFormat == F_PixelFormat_DXT2 ||*/ 
										sourceFormat == F_PixelFormat_DXT3)
									{
										// explicit alpha
										pEAlpha = (DXTExplicitAlphaBlock*)pColor;
										pColor++;
										pInput->Read(pEAlpha, sizeof(DXTExplicitAlphaBlock), 1);
										ImageCodec::FlipEndian(pEAlpha->alphaRow, sizeof(uint16), 4);
										//unpackDXTAlpha(eAlpha, tempColors) ;
									}
									else if (/*sourceFormat == F_PixelFormat_DXT4 || */
										     sourceFormat == F_PixelFormat_DXT5)
									{
										// interpolated alpha
										pIAlpha = (DXTInterpolatedAlphaBlock*)pColor;
										pColor++;
										pInput->Read(pIAlpha, sizeof(DXTInterpolatedAlphaBlock), 1);
										ImageCodec::FlipEndian(&(pIAlpha->alpha_0), sizeof(uint16), 1);
										ImageCodec::FlipEndian(&(pIAlpha->alpha_1), sizeof(uint16), 1);
										//unpackDXTAlpha(*pIAlpha, tempColors) ;
									}
									// always read color
									pInput->Read(pColor,sizeof(DXTColorBlock), 1);
									ImageCodec::FlipEndian(&(pColor->color_0), sizeof(uint16), 1);
									ImageCodec::FlipEndian(&(pColor->color_1), sizeof(uint16), 1);

									RGBA output[16];
									squish::Decompress((squish::u8*)output, block, _DXTColorTypeToSquishType(sourceFormat));
									
									// write 4x4 block to uncompressed version
									for (size_t by = 0; by < 4; ++by)
									{
										for (size_t bx = 0; bx < 4; ++bx)
										{
											uint8* pWiseBit =  (uint8*)&output[by*4+bx];
											FPixelFormat::PackColor(pWiseBit[0], pWiseBit[1], pWiseBit[2], pWiseBit[3], pImageData->ePixelFormat, destPtr);

											destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + destBpp);
										}
										// advance to next row
										destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + destPitchMinus4);
									}
									// next block. Our dest pointer is 4 lines down
									// from where it started
									if (x + 4 == width)
									{
										// Jump back to the start of the line
										destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) - destPitchMinus4);
									}
									else
									{
										// Jump back up 4 rows and 4 pixels to the
										// right to be at the next block to the right
										destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) - dstPitch * 4 + destBpp * 4);
									}
								}
							}
						}
					}
					else
					{
						// load directly
						// DDS format lies! sizeOrPitch is not always set for DXT!!
						size_t dxtSize = FPixelFormat::GetPixelFormatMemorySize(width, height, depth, pImageData->ePixelFormat);
						pInput->Read(destPtr, dxtSize, 1);
						destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + dxtSize);
					}
				}
				else
				{
					// Final data - trim incoming pitch
					size_t srcPitch;
					if (header.flags & DDSD_PITCH)
					{
						srcPitch = header.sizeOrPitch / F_M_MAX((size_t)1, mip * 2);
					}
					else
					{
						// assume same as final pitch
						srcPitch = dstPitch;
					}
					F_Assert(dstPitch <= srcPitch)
					long srcAdvance = static_cast<long>(srcPitch) - static_cast<long>(dstPitch);

					for (int32 z = 0; z < pImageData->nDepth; ++z)
					{
						for (int32 y = 0; y < pImageData->nHeight; ++y)
						{
							pInput->Read(destPtr, dstPitch, 1);
							if (srcAdvance > 0)
								pInput->Seek(srcAdvance, SEEK_CUR);

							destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + dstPitch);
						}
					}
				}

				/// Next mip
				if (width!=1)	width /= 2;
				if (height!=1)	height /= 2;
				if (depth!=1)	depth /= 2;
			}
		}

		FDecodeResult ret;
		ret.first  = pOut;
		ret.second = pImageData;
		return ret;
    }

	ImageCodecDXT::FDecodeResult ImageCodecDXT::Decode(const String& strPath, bool isRGBA) const
	{
		F_Assert(false && "ImageCodecDXT::Decode: Not Implement !")

		FDecodeResult ret;
		ret.first  = nullptr;
		ret.second = nullptr;
		return ret;
	}

    ImageCodecDXT::FDecodeResult ImageCodecDXT::CombineFourTexture(FFileIO* stream0, FFileIO* stream1, FFileIO* stream2, FFileIO* stream3, uint16 mipmaps /*= 0*/)
    {
        // Read 4 character code
		uint32 fileType[4];
		stream0->Read(&fileType[0],sizeof(uint32),1);
		stream1->Read(&fileType[1],sizeof(uint32),1);
		stream2->Read(&fileType[2],sizeof(uint32),1);
		stream3->Read(&fileType[3],sizeof(uint32),1);
		ImageCodec::FlipEndian(&fileType[0],sizeof(uint32),1);
		ImageCodec::FlipEndian(&fileType[1],sizeof(uint32),1);
		ImageCodec::FlipEndian(&fileType[2],sizeof(uint32),1);
		ImageCodec::FlipEndian(&fileType[3],sizeof(uint32),1);

		if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType[0] || 
            F_M_FOURCC('D', 'D', 'S', ' ') != fileType[1] || 
            F_M_FOURCC('D', 'D', 'S', ' ') != fileType[2] || 
            F_M_FOURCC('D', 'D', 'S', ' ') != fileType[3])
		{
			F_LogError("*********************** ImageCodecDXT::CombineFourTexture: This is not a DDS file !");
			F_Assert(false && "ImageCodecDXT::CombineFourTexture: This is not a DDS file !")
			return FDecodeResult();
		}

		// Read header in full
		DDSHeader header[4];
		stream0->Read(&header[0], sizeof(DDSHeader), 1);
		stream1->Read(&header[1], sizeof(DDSHeader), 1);
		stream2->Read(&header[2], sizeof(DDSHeader), 1);
		stream3->Read(&header[3], sizeof(DDSHeader), 1);
		// Endian flip if required, all 32-bit values
		ImageCodec::FlipEndian(&header[0],4,sizeof(DDSHeader) >> 2);
		ImageCodec::FlipEndian(&header[1],4,sizeof(DDSHeader) >> 2);
		ImageCodec::FlipEndian(&header[2],4,sizeof(DDSHeader) >> 2);
		ImageCodec::FlipEndian(&header[3],4,sizeof(DDSHeader) >> 2);

		// Check some sizes
		if (header[0].size != DDS_HEADER_SIZE || 
            header[1].size != DDS_HEADER_SIZE || 
            header[2].size != DDS_HEADER_SIZE || 
            header[3].size != DDS_HEADER_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineFourTexture: DDS header size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineFourTexture: DDS header size mismatch !")
			return FDecodeResult();
		}
		if (header[0].pixelFormat.size != DDS_PIXELFORMAT_SIZE || 
            header[1].pixelFormat.size != DDS_PIXELFORMAT_SIZE || 
            header[2].pixelFormat.size != DDS_PIXELFORMAT_SIZE || 
            header[3].pixelFormat.size != DDS_PIXELFORMAT_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineFourTexture: DDS header pixel size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineFourTexture: DDS header pixel size mismatch !")
			return FDecodeResult();
		}
		//check if texture can combine
		FPixelFormatType sourceFormat[4];
		size_t nPixelBlockSize = 0;
		for (uint16 cnt = 0; cnt < 4; ++cnt)
		{
			if (header[cnt].pixelFormat.flags & DDPF_FOURCC)
			{
				sourceFormat[cnt] = convertFourCCFormat(header[cnt].pixelFormat.fourCC);
			}
			else
			{
				sourceFormat[cnt] = convertPixelFormat(header[cnt].pixelFormat.rgbBits, 
													   header[cnt].pixelFormat.redMask, 
													   header[cnt].pixelFormat.greenMask, 
													   header[cnt].pixelFormat.blueMask, 
													   header[cnt].pixelFormat.flags & DDPF_ALPHAPIXELS ? 
													   header[cnt].pixelFormat.alphaMask : 0);
			}
		}
		if ((sourceFormat[0] & sourceFormat[1] & sourceFormat[2] & sourceFormat[3]) == 0)
		{
			F_LogError("*********************** ImageCodecDXT::CombineFourTexture: DDS format is mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineFourTexture: DDS format is mismatch !")
			return FDecodeResult();
		}

		switch ((int32)sourceFormat[0])
		{
		case F_PixelFormat_DXT1:
			nPixelBlockSize = 8;
			break;
		case F_PixelFormat_DXT2:
		case F_PixelFormat_DXT3:
		case F_PixelFormat_DXT4:
		case F_PixelFormat_DXT5:
			nPixelBlockSize = 16;	
			break;
		default:
			{
				F_LogError("*********************** ImageCodecDXT::CombineFourTexture: Invalid compressed pixel format !");
				F_Assert(false && "ImageCodecDXT::CombineFourTexture: Invalid compressed pixel format !")
				return FDecodeResult();
			}
		}
		//check height of two texture
		uint32 height[4], width[4];
		uint32 nMipMaps = 0;
		uint32 iTemp = 0;
		for (uint16 i = 0; i < 4; ++i)
		{
			height[i] =  header[i].height;
			width[i]  =  header[i].width;
		}
		//check if can combine
		if((height[0]+height[2]) != (height[1]+height[3]) ||
		   (width[0]+width[1]) != (width[2]+width[3]))
		{
			F_LogError("*********************** ImageCodecDXT::CombineFourTexture: DDS height are not same, these two texture can't be combined !");
			F_Assert(false && "ImageCodecDXT::CombineFourTexture: DDS height are not same, these two texture can't be combined !")
			return FDecodeResult();
		}
		if ((header[0].caps.caps1 & DDSCAPS_MIPMAP) && 
			(header[1].caps.caps1 & DDSCAPS_MIPMAP) && 
			(header[2].caps.caps1 & DDSCAPS_MIPMAP) && 
			(header[3].caps.caps1 & DDSCAPS_MIPMAP))
		{
			//max texture size is 4096x4096
			if (mipmaps == 0 || mipmaps > 13)
			{
				iTemp = width[0];
				uint16 tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++nMipMaps;
				}
				iTemp = width[1]; 
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
				iTemp = height[0];
				tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
				iTemp = height[1];
				tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
			}
			else
			{
				nMipMaps = mipmaps;
			}
		}
		
		ImageData* pImageData = new ImageData();
		// only 2D texture be supported
		pImageData->nDepth			= 1; 
		pImageData->nWidth			= width[0] + width[1];
		pImageData->nHeight		    = height[0] + height[2];
		pImageData->nMipmapsCount	= uint16(nMipMaps - 1);
		pImageData->ePixelFormat	= sourceFormat[0];
		pImageData->nFlags		   |= E_ImageFlag_IsCompressed;
		// Calculate total size from number of mipmaps, faces and size
		size_t nPixelSize = Image::CalculateSize((uint32)pImageData->nMipmapsCount, 
                                                 1, 
                                                 (uint32)pImageData->nWidth, 
                                                 (uint32)pImageData->nHeight, 
                                                 (uint32)pImageData->nDepth, 
                                                 pImageData->ePixelFormat);
		pImageData->nSize = (int32)(sizeof(uint32) + sizeof(DDSHeader) + nPixelSize);
		
		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory(pImageData->nSize);
		void* dest = pOut->GetBuffer();
		// write DDS header
		uint32* pFileType = (uint32*)(dest);
		*pFileType        = F_M_FOURCC('D', 'D', 'S', ' ');
		ImageCodec::FlipEndian(pFileType, sizeof(uint32), 1);

		DDSHeader* pHeader = (DDSHeader*)(pFileType+1);
		memset(pHeader,0,sizeof(DDSHeader));
		pHeader->size = DDS_HEADER_SIZE;
		pHeader->pixelFormat.size = DDS_PIXELFORMAT_SIZE;

		pHeader->width  = pImageData->nWidth;
		pHeader->height = pImageData->nHeight;
		pHeader->flags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE;

		if (pImageData->nDepth > 1)
		{
			pHeader->flags  = (pHeader->flags)|DDSD_DEPTH;
			pHeader->depth  = pImageData->nDepth;
		}
		pHeader->mipMapCount		= pImageData->nMipmapsCount + 1;
		pHeader->pixelFormat.fourCC = convertFormatToFourCC(sourceFormat[0]);		
		pHeader->pixelFormat.flags	= DDPF_FOURCC;
		pHeader->sizeOrPitch		= pHeader->width * pHeader->height;
		pHeader->caps.caps1			= DDSCAPS_COMPLEX | DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		if (pImageData->nFlags & E_ImageFlag_IsCubeMap)
		{
			pHeader->caps.caps2 |= DDSCAPS2_CUBEMAP;
		}
		else if (pImageData->nFlags & E_ImageFlag_Is3DTexture)
		{
			pHeader->caps.caps2 |= DDSCAPS2_VOLUME;		
		}
		ImageCodec::FlipEndian(pHeader,4,sizeof(DDSHeader) >> 2);
		//process horizontal texture
		uint8* desData = (uint8*)(pHeader + 1);
		size_t nLineBytes0 = 0;
		size_t nLineBytes1 = 0;
		for (size_t mip = 0; mip < nMipMaps; ++mip)
		{
			size_t bmpHeight = height[0]+height[2];
			
			nLineBytes0 = nPixelBlockSize * ((width[0] + 3) >> 2) ;
			nLineBytes1 = nPixelBlockSize * ((width[1] + 3) >> 2) ;
			for(size_t k =0 ; k<bmpHeight; k+=4)
			{
				FFileIO* readStream0 = ( k<height[0])?stream0:stream2;
				FFileIO* readStream1 = ( k<height[1])?stream1:stream3;

				readStream0->Read(desData,nLineBytes0,1);
				desData += nLineBytes0;
				readStream1->Read(desData,nLineBytes1,1);
				desData += nLineBytes1;
			}
		
			for (uint16 cnt=0; cnt<4; ++cnt)
			{
				if(height[cnt] != 1)  height[cnt] >>= 1;
				if(width[cnt] != 1)   width[cnt] >>= 1;
			}
		}

		FDecodeResult ret;
		ret.first  = pOut;
		ret.second = pImageData;
		return ret;
    }

    ImageCodecDXT::FDecodeResult ImageCodecDXT::CombineTwoTextureHorizontal(FFileIO* stream0, FFileIO* stream1, size_t mipmaps /*= 0*/)
    {
        // Read 4 character code
		uint32 fileType[2];
		stream0->Read(&fileType[0], sizeof(uint32), 1);
		stream1->Read(&fileType[1], sizeof(uint32), 1);
		ImageCodec::FlipEndian(&fileType[0], sizeof(uint32),1);
		ImageCodec::FlipEndian(&fileType[1], sizeof(uint32),1);

		if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType[0] || 
            F_M_FOURCC('D', 'D', 'S', ' ') != fileType[1])
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: This is not a DDS file !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: This is not a DDS file !")
			return FDecodeResult();
		}

		// Read header in full
		DDSHeader header[2];
		stream0->Read(&header[0], sizeof(DDSHeader), 1);
		stream1->Read(&header[1], sizeof(DDSHeader), 1);
		// Endian flip if required, all 32-bit values
		ImageCodec::FlipEndian(&header[0], 4, sizeof(DDSHeader) >> 2);
		ImageCodec::FlipEndian(&header[1], 4, sizeof(DDSHeader) >> 2);

		// Check some sizes
		if (header[0].size != DDS_HEADER_SIZE || header[1].size != DDS_HEADER_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: DDS header size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: DDS header size mismatch !")
			return FDecodeResult();
		}
		if (header[0].pixelFormat.size != DDS_PIXELFORMAT_SIZE || header[1].pixelFormat.size != DDS_PIXELFORMAT_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: DDS header pixel size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: DDS header pixel size mismatch !")
			return FDecodeResult();
		}
		//check if texture can combine
		FPixelFormatType sourceFormat[2];
		size_t nPixelBlockSize = 0;
		for (uint16 cnt = 0; cnt < 2; ++cnt)
		{
			if (header[cnt].pixelFormat.flags & DDPF_FOURCC)
			{
				sourceFormat[cnt] = convertFourCCFormat(header[cnt].pixelFormat.fourCC);
			}
			else
			{
				sourceFormat[cnt] = convertPixelFormat(header[cnt].pixelFormat.rgbBits, 
													   header[cnt].pixelFormat.redMask,
													   header[cnt].pixelFormat.greenMask, 
													   header[cnt].pixelFormat.blueMask, 
													   header[cnt].pixelFormat.flags & DDPF_ALPHAPIXELS ? 
													   header[cnt].pixelFormat.alphaMask : 0);
			}

		}
		if ((sourceFormat[0] & sourceFormat[1]) == 0)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: DDS format is mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: DDS format is mismatch !")
			return FDecodeResult();
		}
		switch ((int32)sourceFormat[0])
		{
		case F_PixelFormat_DXT1:
			nPixelBlockSize = 8;
			break;
		case F_PixelFormat_DXT2:
		case F_PixelFormat_DXT3:
		case F_PixelFormat_DXT4:
		case F_PixelFormat_DXT5:
			nPixelBlockSize = 16;	
			break;
		default:
			{
				F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: Invalid compressed pixel format !");
				F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: Invalid compressed pixel format !")
				return FDecodeResult();
			}
		}
		//check height of two texture
		uint32 height[2], width[2];
		uint32 nMipMaps = 0;
		uint32 iTemp    = 0;
		for (uint16 i = 0; i < 2; ++i)
		{
			height[i] =  header[i].height;
			width[i]  =  header[i].width;
		}
		//check if can combine
		if ((height[0] != height[1]))
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureHorizontal: DDS height are not same, these two texture can't be combined !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureHorizontal: DDS height are not same, these two texture can't be combined !")
			return FDecodeResult();
		}
		if ((header[0].caps.caps1 & DDSCAPS_MIPMAP) && (header[1].caps.caps1 & DDSCAPS_MIPMAP))
		{
			//max texture size is 4096x4096
			if (mipmaps == 0 || mipmaps > 13)
			{
				iTemp = width[0];
				uint16 tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++nMipMaps;
				}
				iTemp = width[1]; 
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
				iTemp = height[0];
				tmpMipMaps = 0;
				while((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
			}
			else
			{
				nMipMaps = (uint32)mipmaps;
			}
		}
		
		ImageData* pImageData = new ImageData();
		// only 2D texture be supported
		pImageData->nDepth		  = 1; 
		pImageData->nWidth		  = width[0] + width[1];
		pImageData->nHeight		  = height[0];
		pImageData->nMipmapsCount = uint16(nMipMaps - 1);
		pImageData->ePixelFormat  = sourceFormat[0];
		pImageData->nFlags		 |= E_ImageFlag_IsCompressed;
		// Calculate total size from number of mipmaps, faces and size
		uint32 nPixelSize = Image::CalculateSize((uint32)pImageData->nMipmapsCount,   
                                                 1, 
                                                 (uint32)pImageData->nWidth, 
                                                 (uint32)pImageData->nHeight, 
                                                 (uint32)pImageData->nDepth, 
                                                 pImageData->ePixelFormat);
		pImageData->nSize = (int32)(sizeof(uint32) + sizeof(DDSHeader) + nPixelSize);

		// buffer
		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory(pImageData->nSize);
		void* dest = pOut->GetBuffer();
		// write DDS header
		uint32* pFileType = (uint32*)(dest);
		*pFileType        = F_M_FOURCC('D', 'D', 'S', ' ');
		ImageCodec::FlipEndian(pFileType,sizeof(uint32),1);

		DDSHeader* pHeader = (DDSHeader*)(pFileType+1);
		memset(pHeader,0,sizeof(DDSHeader));
		pHeader->size = DDS_HEADER_SIZE;
		pHeader->pixelFormat.size = DDS_PIXELFORMAT_SIZE;

		pHeader->width  = pImageData->nWidth ;
		pHeader->height = pImageData->nHeight;
		pHeader->flags   = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE;
		if (pImageData->nDepth > 1)
		{
			pHeader->flags  = (pHeader->flags)|DDSD_DEPTH;
			pHeader->depth = pImageData->nDepth;
		}
		pHeader->mipMapCount		= pImageData->nMipmapsCount + 1;
		pHeader->pixelFormat.fourCC = convertFormatToFourCC(sourceFormat[0]);		
		pHeader->pixelFormat.flags	= DDPF_FOURCC;
		pHeader->sizeOrPitch		= pHeader->width * pHeader->height;
		pHeader->caps.caps1			= DDSCAPS_COMPLEX | DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		if (pImageData->nFlags & E_ImageFlag_IsCubeMap)
		{
			pHeader->caps.caps2 |= DDSCAPS2_CUBEMAP;
		}
		else if (pImageData->nFlags & E_ImageFlag_Is3DTexture)
		{
			pHeader->caps.caps2 |= DDSCAPS2_VOLUME;
		}
		ImageCodec::FlipEndian(pHeader, 4, sizeof(DDSHeader) >> 2);
		//process horizontal texture
		uint8* desData = (uint8*)(pHeader + 1);
		size_t nLineBytes0 = 0;
		size_t nLineBytes1 = 0;
		for (size_t mip = 0; mip < nMipMaps; ++mip)
		{
			nLineBytes0 = nPixelBlockSize * ((width[0] + 3) >> 2);
			nLineBytes1 = nPixelBlockSize * ((width[1] + 3) >> 2);
			for (size_t i = 0; i < height[0]; i+=4)
			{
				//copy data block	
				stream0->Read(desData, nLineBytes0, 1);
				desData += nLineBytes0;
				stream1->Read(desData, nLineBytes1, 1);
				desData += nLineBytes1;
			}
			//
			for (uint16 cnt = 0; cnt < 2; ++cnt)
			{
				if(height[cnt] != 1)  height[cnt] >>= 1;
				if(width[cnt] != 1)   width[cnt] >>= 1;
			}
		}

		FDecodeResult ret;
		ret.first  = pOut;
		ret.second = pImageData;
		return ret;
    }

    ImageCodecDXT::FDecodeResult ImageCodecDXT::CombineTwoTextureVertical(FFileIO* stream0, FFileIO* stream1, size_t mipmaps /*= 0*/)
    {
        // Read 4 character code
		uint32 fileType[2];
		stream0->Read(&fileType[0], sizeof(uint32), 1);
		stream1->Read(&fileType[1], sizeof(uint32), 1);
		ImageCodec::FlipEndian(&fileType[0], sizeof(uint32), 1);
		ImageCodec::FlipEndian(&fileType[1], sizeof(uint32), 1);

		if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType[0] || 
            F_M_FOURCC('D', 'D', 'S', ' ') != fileType[1])
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: This is not a DDS file !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: This is not a DDS file !")
			return FDecodeResult();
		}

		// Read header in full
		DDSHeader header[4];
		stream0->Read(&header[0], sizeof(DDSHeader), 1);
		stream1->Read(&header[1], sizeof(DDSHeader), 1);
		// Endian flip if required, all 32-bit values
		ImageCodec::FlipEndian(&header[0], 4, sizeof(DDSHeader) >> 2);
		ImageCodec::FlipEndian(&header[1], 4, sizeof(DDSHeader) >> 2);

		// Check some sizes
		if (header[0].size != DDS_HEADER_SIZE || header[1].size != DDS_HEADER_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: DDS header size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: DDS header size mismatch !")
			return FDecodeResult();
		}
		if (header[0].pixelFormat.size != DDS_PIXELFORMAT_SIZE || header[1].pixelFormat.size != DDS_PIXELFORMAT_SIZE)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: DDS header pixel size mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: DDS header pixel size mismatch !")
			return FDecodeResult();
		}
		//check if texture can combine
		FPixelFormatType sourceFormat[2];
		size_t nPixelBlockSize = 0;
		for (uint16 cnt = 0; cnt < 2; ++cnt)
		{
			if (header[cnt].pixelFormat.flags & DDPF_FOURCC)
			{
				sourceFormat[cnt] = convertFourCCFormat(header[cnt].pixelFormat.fourCC);
			}
			else
			{
				sourceFormat[cnt] = convertPixelFormat(header[cnt].pixelFormat.rgbBits, 
													   header[cnt].pixelFormat.redMask, 
													   header[cnt].pixelFormat.greenMask, 
													   header[cnt].pixelFormat.blueMask, 
													   header[cnt].pixelFormat.flags & DDPF_ALPHAPIXELS ? 
													   header[cnt].pixelFormat.alphaMask : 0);
			}

		}
		if ((sourceFormat[0] & sourceFormat[1] ) == 0)
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: DDS format is mismatch !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: DDS format is mismatch !")
			return FDecodeResult();
		}
		switch ((int32)sourceFormat[0])
		{
		case F_PixelFormat_DXT1:
			nPixelBlockSize = 8;
			break;
		case F_PixelFormat_DXT2:
		case F_PixelFormat_DXT3:
		case F_PixelFormat_DXT4:
		case F_PixelFormat_DXT5:
			nPixelBlockSize = 16;	
			break;
		default:
			{
				F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: Invalid compressed pixel format !");
				F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: Invalid compressed pixel format !")
				return FDecodeResult();
			}
		}
		//check height of two texture
		uint32 height[2], width[2];
		uint32 nMipMaps = 0;
		uint32 iTemp      = 0;
		for (uint16 i = 0; i < 2; ++i)
		{
			height[i] = header[i].height;
			width[i]  = header[i].width;
		}
		//check if can combine
		if ((width[0] != width[1]))
		{
			F_LogError("*********************** ImageCodecDXT::CombineTwoTextureVertical: DDS height are not same, these two texture can't be combined !");
			F_Assert(false && "ImageCodecDXT::CombineTwoTextureVertical: DDS height are not same, these two texture can't be combined !")
			return FDecodeResult();
		}
		if ((header[0].caps.caps1 & DDSCAPS_MIPMAP) && (header[1].caps.caps1 & DDSCAPS_MIPMAP))
		{
			//max texture size is 4096x4096
			if (mipmaps == 0 || mipmaps > 13)
			{
				//calculate mipmap level
				iTemp = width[0];
				uint16 tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++nMipMaps;
				}
				iTemp			 = height[0];
				tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
				iTemp = height[1];
				tmpMipMaps = 0;
				while ((iTemp%4) == 0)
				{
					iTemp >>= 1;
					++tmpMipMaps;
				}
				nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
			}
			else
			{
				nMipMaps = (uint32)mipmaps;
			}
		}
		
		ImageData* pImageData = new ImageData();
		// only 2D texture be supported
		pImageData->nDepth		  = 1; 
		pImageData->nWidth		  = width[0];
		pImageData->nHeight		  = height[0] + height[1];
		pImageData->nMipmapsCount = uint16(nMipMaps - 1);
		pImageData->ePixelFormat  = sourceFormat[0];
		pImageData->nFlags		 |= E_ImageFlag_IsCompressed;
		// Calculate total size from number of mipmaps, faces and size
		uint32 nPixelSize = Image::CalculateSize((uint32)pImageData->nMipmapsCount,
                                                 1, 
                                                 (uint32)pImageData->nWidth, 
                                                 (uint32)pImageData->nHeight, 
                                                 (uint32)pImageData->nDepth, 
                                                 pImageData->ePixelFormat);
		pImageData->nSize = (int32)(sizeof(uint32) + sizeof(DDSHeader) + nPixelSize);
		// buffer	
		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory(pImageData->nSize);
		void* dest = pOut->GetBuffer();
		// write DDS header
		uint32* pFileType = (uint32*)(dest);
		*pFileType        = F_M_FOURCC('D', 'D', 'S', ' ');
		ImageCodec::FlipEndian(pFileType, sizeof(uint32), 1);

		DDSHeader* pHeader = (DDSHeader*)(pFileType+1);
		memset(pHeader,0,sizeof(DDSHeader));
		pHeader->size = DDS_HEADER_SIZE;
		pHeader->pixelFormat.size = DDS_PIXELFORMAT_SIZE;

		pHeader->width  = pImageData->nWidth;
		pHeader->height = pImageData->nHeight;
		pHeader->flags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE;

		if (pImageData->nDepth > 1)
		{
			pHeader->flags = (pHeader->flags)|DDSD_DEPTH;
			pHeader->depth = pImageData->nDepth;
		}
		pHeader->mipMapCount		= pImageData->nMipmapsCount + 1;
		pHeader->pixelFormat.fourCC = convertFormatToFourCC(sourceFormat[0]);		
		pHeader->pixelFormat.flags	= DDPF_FOURCC;
		pHeader->sizeOrPitch		= pHeader->width * pHeader->height;
		pHeader->caps.caps1			= DDSCAPS_COMPLEX | DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		if (pImageData->nFlags & E_ImageFlag_IsCubeMap)
		{
			pHeader->caps.caps2 |= DDSCAPS2_CUBEMAP;
		}
		else if (pImageData->nFlags & E_ImageFlag_Is3DTexture)
		{
			pHeader->caps.caps2 |= DDSCAPS2_VOLUME;		
		}
		ImageCodec::FlipEndian(pHeader, 4, sizeof(DDSHeader) >> 2);
		//process horizontal texture
		uint8* desData = (uint8*)(pHeader + 1);
		size_t nLineBytes0 = 0;
		size_t nLineBytes1 = 0;
		for (size_t mip = 0; mip < nMipMaps; ++mip)
		{
			//process vertical texture
			nLineBytes0 = nPixelBlockSize * ((width[0] + 3) * height[0] >> 2);
			nLineBytes0 = nPixelBlockSize * ((width[0] + 3) * height[1] >> 2);
			//copy data block
			stream0->Read(desData, nLineBytes0, 1);
			desData += nLineBytes0;
			stream1->Read(desData, nLineBytes1, 1);
			desData += nLineBytes1;
			//
			for (uint16 cnt = 0; cnt < 2; ++cnt)
			{
				if(height[cnt] != 1)  height[cnt] >>= 1;
				if(width[cnt] != 1)   width[cnt] >>= 1;
			}
		}

		FDecodeResult ret;
		ret.first  = pOut;
		ret.second = pImageData;
		return ret;
    }

    ImageCodecDXT::FDecodeResult ImageCodecDXT::CombineTexturesInSameSize(FFileIOPtrVector stream, uint16 mipmaps /*= 0*/)
    {
    #define Max_Combine_Tex 20
		uint16 num = (uint16)stream.size();
		DDSHeader header[Max_Combine_Tex];
		for (uint16 i = 0; i < num; ++i)
		{
			uint32 fileType;
			stream[i]->Read(&fileType, sizeof(uint32), 1);
			ImageCodec::FlipEndian(&fileType, sizeof(uint32), 1);

			if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType)
			{
				F_LogError("*********************** ImageCodecDXT::CombineTexturesInSameSize: This is not a DDS file !");
				F_Assert(false && "ImageCodecDXT::CombineTexturesInSameSize: This is not a DDS file !")
				return FDecodeResult();
			}

			// Read header in full
			stream[i]->Read(&header[i], sizeof(DDSHeader), 1);
			// Endian flip if required, all 32-bit values
			ImageCodec::FlipEndian(&header[i],4,sizeof(DDSHeader) / 4);
			// Check some sizes
			if (header[i].size != DDS_HEADER_SIZE )
			{
				F_LogError("*********************** ImageCodecDXT::CombineTexturesInSameSize: DDS header size mismatch !");
				F_Assert(false && "ImageCodecDXT::CombineTexturesInSameSize: DDS header size mismatch !")
				return FDecodeResult();
			}
			if (header[i].pixelFormat.size != DDS_PIXELFORMAT_SIZE )
			{
				F_LogError("*********************** ImageCodecDXT::CombineTexturesInSameSize: DDS header pixel size mismatch !");
				F_Assert(false && "ImageCodecDXT::CombineTexturesInSameSize: DDS header pixel size mismatch !")
				return FDecodeResult();
			}
		}

		//check if texture can combine
		FPixelFormatType sourceFormat[Max_Combine_Tex];
		size_t nPixelBlockSize = 0;
		size_t format = 0xffff;
		for (uint16 cnt = 0; cnt < num; ++cnt)
		{
			if (header[cnt].pixelFormat.flags & DDPF_FOURCC)
			{
				sourceFormat[cnt] = convertFourCCFormat(header[cnt].pixelFormat.fourCC);
			}
			else
			{
				sourceFormat[cnt] = convertPixelFormat(header[cnt].pixelFormat.rgbBits, 
													   header[cnt].pixelFormat.redMask, 
													   header[cnt].pixelFormat.greenMask, 
													   header[cnt].pixelFormat.blueMask, 
													   header[cnt].pixelFormat.flags & DDPF_ALPHAPIXELS ? 
													   header[cnt].pixelFormat.alphaMask : 0);
			}
			if ((format &=sourceFormat[cnt]) == 0)
			{
				F_LogError("*********************** ImageCodecDXT::CombineTexturesInSameSize: DDS format is mismatch !");
				F_Assert(false && "ImageCodecDXT::CombineTexturesInSameSize: DDS format is mismatch !")
				return FDecodeResult();
			}
		}

		switch ((int32)sourceFormat[0])
		{
		case F_PixelFormat_DXT1:
			nPixelBlockSize = 8;
			break;
		case F_PixelFormat_DXT2:
		case F_PixelFormat_DXT3:
		case F_PixelFormat_DXT4:
		case F_PixelFormat_DXT5:
			nPixelBlockSize = 16;	
			break;
		default:
			{
				F_LogError("*********************** ImageCodecDXT::CombineTexturesInSameSize: Invalid compressed pixel format !");
				F_Assert(false && "ImageCodecDXT::CombineTexturesInSameSize: Invalid compressed pixel format !")
				return FDecodeResult();
			}
		}
		//check if can combine
		bool bUseMipmap = true;
		for (uint16 i = 0; i < num; ++i)
		{
			if (!(header[i].caps.caps1 & DDSCAPS_MIPMAP))
			{
				bUseMipmap = false;
				break;
			}
		}
		uint32 nMipMaps = 0;
		uint32 height, width;
		if (bUseMipmap)
		{
			for (uint16 i = 0; i < num; ++i)
			{
				uint32 iTemp      = 0;
				height =  header[i].height;
				width  =  header[i].width;
				if (mipmaps == 0 || mipmaps > 13)
				{
					iTemp = width;
					uint16 tmpMipMaps = 0;
					while ((iTemp%4) == 0)
					{
						iTemp >>= 1;
						++nMipMaps;
					}
					iTemp = height; 
					while ((iTemp%4) == 0)
					{
						iTemp >>= 1;
						++tmpMipMaps;
					}
					nMipMaps = nMipMaps < tmpMipMaps ? nMipMaps : tmpMipMaps;
				}
				else
				{
					nMipMaps = mipmaps;
				}
			}
		}
		else
		{
			height = header[0].height;
			width = header[0].width;
		}
		// create combine texture
		int s = 1;
		while (s)
		{
			if(s * s >= num)
				break;
			++s;
		}
		ImageData* pImageData = new ImageData();
		// only 2D texture be supported
		pImageData->nDepth		  = 1; 
		pImageData->nWidth	      = width * s;
		pImageData->nHeight		  = height * s;
		pImageData->nMipmapsCount = uint16(nMipMaps - 1);
		pImageData->ePixelFormat  = sourceFormat[0];
		pImageData->nFlags		 |= E_ImageFlag_IsCompressed;
		// Calculate total size from number of mipmaps, faces and size
		uint32 nPixelSize = Image::CalculateSize((uint32)pImageData->nMipmapsCount, 
                                                 1, 
                                                 (uint32)pImageData->nWidth, 
                                                 (uint32)pImageData->nHeight, 
                                                 (uint32)pImageData->nDepth, 
                                                 pImageData->ePixelFormat);
		pImageData->nSize = (int32)(sizeof(uint32) + sizeof(DDSHeader) + nPixelSize);
		
		FFileMemory* pOut = FFileManager::GetSingleton().CreateFileMemory(pImageData->nSize);
		void* dest = pOut->GetBuffer();
		// write DDS header
		uint32* pFileType = (uint32*)(dest);
		*pFileType        = F_M_FOURCC('D', 'D', 'S', ' ');
		ImageCodec::FlipEndian(pFileType,sizeof(uint32),1);

		DDSHeader* pHeader = (DDSHeader*)(pFileType+1);
		memset(pHeader,0,sizeof(DDSHeader));
		pHeader->size = DDS_HEADER_SIZE;
		pHeader->pixelFormat.size = DDS_PIXELFORMAT_SIZE;

		pHeader->width  = pImageData->nWidth ;
		pHeader->height = pImageData->nHeight;
		pHeader->flags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE;

		if (pImageData->nDepth > 1)
		{
			pHeader->flags  = (pHeader->flags)|DDSD_DEPTH;
			pHeader->depth  = pImageData->nDepth;
		}
		pHeader->mipMapCount		= pImageData->nMipmapsCount + 1;
		pHeader->pixelFormat.fourCC = convertFormatToFourCC(sourceFormat[0]);		
		pHeader->pixelFormat.flags	= DDPF_FOURCC;
		pHeader->sizeOrPitch		= pHeader->width * pHeader->height;
		pHeader->caps.caps1			= DDSCAPS_COMPLEX | DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		if (pImageData->nFlags & E_ImageFlag_IsCubeMap)
		{
			pHeader->caps.caps2 |= DDSCAPS2_CUBEMAP;
		}
		else if (pImageData->nFlags & E_ImageFlag_Is3DTexture)
		{
			pHeader->caps.caps2 |= DDSCAPS2_VOLUME;		
		}
		ImageCodec::FlipEndian(pHeader, 4, sizeof(DDSHeader) >> 2);

		//process  textures
		uint8* desData = (uint8*)(pHeader + 1);
		size_t nLineBytes = 0;
		for (size_t mip = 0; mip < nMipMaps; ++mip)
		{
			for (int i = 0; i < s; ++i)
			{
				nLineBytes = nPixelBlockSize * ((width + 3) >> 2);
				for (size_t k = 0; k < height; k+=4)
				{
					for(int j = 0; j < s; ++j)
					{
						if (i * s + j < num)
						{
							stream[i*s+j]->Read(desData, nLineBytes, 1);
						}
						desData += nLineBytes;
					}
				}
			}
			
			for (uint16 cnt = 0; cnt < num; ++cnt)
			{
				if(height != 1)  height >>= 1;
				if(width != 1)   width >>= 1;
			}
		}

		FDecodeResult ret;
		ret.first  = pOut;
		ret.second = pImageData;
		return ret;
    }   

    bool ImageCodecDXT::CheckCombineTextures(FFileIO* stream, uint16 num, size_t* height, size_t* width, size_t* pixelblocksize, size_t* mipmap)
    {
        // Read 4 character code
    #define Max_Combine_Textures 6
		uint32 fileType[Max_Combine_Textures];
		DDSHeader header[Max_Combine_Textures];
		size_t nMaxMipMaps = 20;
		for (uint16 i=0; i<num;++i)
		{
			stream[i].Read(&fileType[i],sizeof(uint32),1);
			ImageCodec::FlipEndian(&fileType[i],sizeof(uint32),1);

			if (F_M_FOURCC('D', 'D', 'S', ' ') != fileType[i])
			{
				F_LogError("*********************** ImageCodecDXT::CheckCombineTextures: This is not a DDS file !");
				F_Assert(false && "ImageCodecDXT::CheckCombineTextures: This is not a DDS file !")
				return false;
			}

			// Read header in full
			stream[i].Read(&header[i],sizeof(DDSHeader),1);
			// Endian flip if required, all 32-bit values
			ImageCodec::FlipEndian(&header[i], 4, sizeof(DDSHeader)/4);
			// Check some sizes
			if (header[i].size != DDS_HEADER_SIZE)
			{
				F_LogError("*********************** ImageCodecDXT::CheckCombineTextures: DDS header size mismatch !");
				F_Assert(false && "ImageCodecDXT::CheckCombineTextures: DDS header size mismatch !")
				return false;
			}
			if (header[i].pixelFormat.size != DDS_PIXELFORMAT_SIZE)
			{
				F_LogError("*********************** ImageCodecDXT::CheckCombineTextures: DDS header pixel size mismatch !");
				F_Assert(false && "ImageCodecDXT::CheckCombineTextures: DDS header pixel size mismatch !")
				return false;
			}
			if ((header[i].caps.caps1 & DDSCAPS_MIPMAP))
			{
				nMaxMipMaps = nMaxMipMaps < header[i].mipMapCount ? nMaxMipMaps : header[i].mipMapCount;
			}

			*mipmap = nMaxMipMaps - 1;
		}
		
		//check if texture can combine
		FPixelFormatType sourceFormat[Max_Combine_Textures];
		size_t format = 0xffff;
		for (uint16 cnt = 0; cnt < num; ++cnt)
		{
			if (header[cnt].pixelFormat.flags & DDPF_FOURCC)
			{
				sourceFormat[cnt] = convertFourCCFormat(header[cnt].pixelFormat.fourCC);
			}
			else
			{
				sourceFormat[cnt] = convertPixelFormat(header[cnt].pixelFormat.rgbBits, 
													   header[cnt].pixelFormat.redMask, 
													   header[cnt].pixelFormat.greenMask, 
													   header[cnt].pixelFormat.blueMask, 
													   header[cnt].pixelFormat.flags & DDPF_ALPHAPIXELS ? 
													   header[cnt].pixelFormat.alphaMask : 0);
			}
			if ((format &=sourceFormat[cnt]) == 0)
			{
				F_LogError("*********************** ImageCodecDXT::CheckCombineTextures: DDS format is mismatch !");
				F_Assert(false && "ImageCodecDXT::CheckCombineTextures: DDS format is mismatch !")
				return false;
			}
		}
		
		switch ((int32)sourceFormat[0])
		{
		case F_PixelFormat_DXT1:
			*pixelblocksize = 8;
			break;
		case F_PixelFormat_DXT2:
		case F_PixelFormat_DXT3:
		case F_PixelFormat_DXT4:
		case F_PixelFormat_DXT5:
			*pixelblocksize = 16;	
			break;
		default:
			{
				F_LogError("*********************** ImageCodecDXT::CheckCombineTextures: Invalid compressed pixel format !");
				F_Assert(false && "ImageCodecDXT::CheckCombineTextures: Invalid compressed pixel format !")
				return false;
			}
		}
		//check height of two texture
		for (uint16 i=0; i<num; ++i)
		{
			height[i] =  header[i].height;
			width[i]  =  header[i].width;
		}
		return true;
    }

    FPixelFormatType ImageCodecDXT::convertFourCCFormat(uint32 fourcc) const
    {
        // convert dxt pixel format
		switch (fourcc)
		{
		case F_M_FOURCC('D','X','T','1'):
			return F_PixelFormat_DXT1;
		case F_M_FOURCC('D','X','T','2'):
			return F_PixelFormat_DXT2;
		case F_M_FOURCC('D','X','T','3'):
			return F_PixelFormat_DXT3;
		case F_M_FOURCC('D','X','T','4'):
			return F_PixelFormat_DXT4;
		case F_M_FOURCC('D','X','T','5'):
			return F_PixelFormat_DXT5;
		case D3DFMT_R16F:
			return F_PixelFormat_SHORT_R16_SFLOAT;
		case D3DFMT_G16R16F:
			return F_PixelFormat_SHORT_G16R16_SFLOAT;
		case D3DFMT_A16B16G16R16F:
			return F_PixelFormat_SHORT_A16B16G16R16_SFLOAT;
		case D3DFMT_R32F:
			return F_PixelFormat_INT_R32_SFLOAT;
		case D3DFMT_G32R32F:
			return F_PixelFormat_INT_G32R32_SFLOAT;
		case D3DFMT_A32B32G32R32F:
			return F_PixelFormat_INT_A32B32G32R32_SFLOAT;
			// We could support 3Dc here, but only ATI cards support it, not nVidia
		default:
			{
				F_LogError("*********************** ImageCodecDXT::convertFourCCFormat: Unsupported FourCC format found in DDS file !");
				F_Assert(false && "ImageCodecDXT::convertFourCCFormat: Unsupported FourCC format found in DDS file !")
			}
		};

		return F_PixelFormat_Unknown;
    }

    FPixelFormatType ImageCodecDXT::convertPixelFormat(uint32 rgbBits, uint32 rMask, uint32 gMask, uint32 bMask, uint32 aMask) const
    {
        // General search through pixel formats
		for (int i = F_PixelFormat_Unknown + 1; i < F_PixelFormat_Count; ++i)
		{
			FPixelFormatType ePixelFormat = static_cast<FPixelFormatType>(i);
			if (FPixelFormat::GetPixelFormatElemBits(ePixelFormat) == rgbBits)
			{
				uint64 testMasks[4];
				FPixelFormat::GetPixelFormatBitMasks(ePixelFormat, testMasks);
				int testBits[4];
				FPixelFormat::GetPixelFormatBitDepths(ePixelFormat, testBits);
				if (testMasks[0] == rMask && 
					testMasks[1] == gMask &&
					testMasks[2] == bMask && 
					// for alpha, deal with 'X8' formats by checking bit counts
					(testMasks[3] == aMask || (aMask == 0 && testBits[3] == 0)))
				{
					return ePixelFormat;
				}
			}
		}

		F_LogError("*********************** ImageCodecDXT::convertPixelFormat: Cannot determine pixel format !");
		F_Assert(false && "ImageCodecDXT::convertPixelFormat: Cannot determine pixel format !")
		return F_PixelFormat_Unknown;
    }

    uint32 ImageCodecDXT::convertFormatToFourCC(FPixelFormatType format) const
    {
        switch ((int32)format)
		{
		case F_PixelFormat_DXT1:
			return F_M_FOURCC('D','X','T','1');
		case F_PixelFormat_DXT2:
			return F_M_FOURCC('D','X','T','2');
		case F_PixelFormat_DXT3:
			return F_M_FOURCC('D','X','T','3');
		case F_PixelFormat_DXT4:
			return F_M_FOURCC('D','X','T','4');
		case F_PixelFormat_DXT5:
			return F_M_FOURCC('D','X','T','5');
		case F_PixelFormat_SHORT_R16_SFLOAT:
			return D3DFMT_R16F;
		case F_PixelFormat_SHORT_G16R16_SFLOAT:
			return D3DFMT_G16R16F;
		case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
			return D3DFMT_A16B16G16R16F;
		case F_PixelFormat_INT_R32_SFLOAT:
			return D3DFMT_R32F;
		case F_PixelFormat_INT_G32R32_SFLOAT:
			return D3DFMT_G32R32F;
		case F_PixelFormat_INT_A32B32G32R32_SFLOAT:
			return D3DFMT_A32B32G32R32F;
			// We could support 3Dc here, but only ATI cards support it, not nVidia
		default:
			{
				F_LogError("*********************** ImageCodecDXT::convertFormatToFourCC: Unsupported FourCC format found in DDS file !");
				F_Assert(false && "ImageCodecDXT::convertFormatToFourCC: Unsupported FourCC format found in DDS file !")
			}
		};
		
		return F_PixelFormat_Unknown;
    }

    void ImageCodecDXT::unpackDXTColor(FPixelFormatType ePixelFormat, const DXTColorBlock& block, FColor* pCol) const
    {
        // Note - we assume all values have already been endian swapped
		FColor derivedColors[4];
		if (ePixelFormat == F_PixelFormat_DXT1 && block.color_0 <= block.color_1)
		{
			// 1-bit alpha
			FPixelFormat::UnpackColor(&(derivedColors[0]), F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16, &(block.color_0));
			FPixelFormat::UnpackColor(&(derivedColors[1]), F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16, &(block.color_1));
			// one intermediate color, half way between the other two
			derivedColors[2] = (derivedColors[0] + derivedColors[1]) / 2.0f;
			// transparent color
			derivedColors[3] = FColor(0,0,0,0);
		}
		else
		{
			FPixelFormat::UnpackColor(&(derivedColors[0]), F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16, &(block.color_0));
			FPixelFormat::UnpackColor(&(derivedColors[1]), F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16, &(block.color_1));
			// first interpolated color, 1/3 of the way along
			derivedColors[2] = (2.0f * derivedColors[0] + derivedColors[1]) / 3.0f;
			// second interpolated color, 2/3 of the way along
			derivedColors[3] = (derivedColors[0] + 2.0f * derivedColors[1]) / 3.0f;
		}

		// Process 4x4 block of texels
		for (size_t row = 0; row < 4; ++row)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				// LSB come first
				uint8 colIdx = static_cast<uint8>(block.indexRow[row] >> (x * 2) & 0x3);
				if (ePixelFormat == F_PixelFormat_DXT1)
				{
					// Overwrite entire color
					pCol[(row * 4) + x] = derivedColors[colIdx];
				}
				else
				{
					// alpha has already been read (alpha precedes color)
					FColor& col = pCol[(row * 4) + x];
					col.r = derivedColors[colIdx].r;
					col.g = derivedColors[colIdx].g;
					col.b = derivedColors[colIdx].b;
				}
			}
		}
    }

    void ImageCodecDXT::unpackDXTAlpha(const DXTExplicitAlphaBlock& block, FColor* pCol) const
    {
        // Note - we assume all values have already been endian swapped
		// This is an explicit alpha block, 4 bits per pixel, LSB first
		for (size_t row = 0; row < 4; ++row)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				// Shift and mask off to 4 bits
				uint8 val = static_cast<uint8>(block.alphaRow[row] >> (x * 4) & 0xF);
				// Convert to [0,1]
				pCol->a = (float)val / (float)0xF;
				pCol++;
			}
		}
    }

    void ImageCodecDXT::unpackDXTAlpha(const DXTInterpolatedAlphaBlock& block, FColor* pCol) const
    {
        // 8 derived alpha values to be indexed
		float derivedAlphas[8];

		// Explicit extremes
		derivedAlphas[0] = block.alpha_0 / (float)0xFF;
		derivedAlphas[1] = block.alpha_1 / (float)0xFF;

		if (block.alpha_0 <= block.alpha_1)
		{
			// 4 interpolated alphas, plus zero and one			
			// full range including extremes at [0] and [5]
			// we want to fill in [1] through [4] at weights ranging
			// from 1/5 to 4/5
			float denom = 1.0f / 5.0f;
			for (size_t i = 0; i < 4; ++i) 
			{
				float factor0 = (4 - i) * denom;
				float factor1 = (i + 1) * denom;
				derivedAlphas[i + 2] = 
					(factor0 * block.alpha_0) + (factor1 * block.alpha_1);
			}
			derivedAlphas[6] = 0.0f;
			derivedAlphas[7] = 1.0f;
		}
		else
		{
			// 6 interpolated alphas
			// full range including extremes at [0] and [7]
			// we want to fill in [1] through [6] at weights ranging
			// from 1/7 to 6/7
			float denom = 1.0f / 7.0f;
			for (size_t i = 0; i < 6; ++i) 
			{
				float factor0 = (6 - i) * denom;
				float factor1 = (i + 1) * denom;
				derivedAlphas[i + 2] = 
					(factor0 * block.alpha_0) + (factor1 * block.alpha_1);
			}
		}

		// Ok, now we've built the reference values, process the indexes
		for (size_t i = 0; i < 16; ++i)
		{
			size_t baseByte = (i * 3) / 8;
			size_t baseBit = (i * 3) % 8;
			uint8 bits = static_cast<uint8>(block.indexes[baseByte] >> baseBit & 0x7);
			// do we need to stitch in next byte too?
			if (baseBit > 5)
			{
				uint8 extraBits = static_cast<uint8>(
					(block.indexes[baseByte+1] << (8 - baseBit)) & 0xFF);
				bits |= extraBits & 0x7;
			}
			pCol[i].a = derivedAlphas[bits];
		}
    }
    
    int	ImageCodecDXT::_DXTColorTypeToSquishType(FPixelFormatType format) const
    {
        int iType = 0;
		switch ((int32)format)
		{
		case F_PixelFormat_DXT1:
			iType = squish::kDxt1 |squish::kColourIterativeClusterFit | squish::kColourClusterFit;
			break;
		case F_PixelFormat_DXT3:
			iType = squish::kDxt3 |squish::kColourIterativeClusterFit | squish::kColourClusterFit;
			break;
		case F_PixelFormat_DXT5:
			iType = squish::kDxt5 |squish::kColourIterativeClusterFit | squish::kColourClusterFit;
			break;
		}

		return iType;
    }
    
}; //LostPeterEngine