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

#ifndef _IMAGE_CODEC_DXT_H_
#define _IMAGE_CODEC_DXT_H_

#include "ImageCodec.h"

namespace LostPeterEngine
{
    struct DXTColorBlock;
	struct DXTExplicitAlphaBlock;
	struct DXTInterpolatedAlphaBlock;
	class  DDSCodecToolImpl;
	struct DDSHeader;

    class engineExport ImageCodecDXT : public ImageCodec
    {
    public:
        ImageCodecDXT();
        virtual ~ImageCodecDXT();

    public:
    private:
		String m_strType;

		static ImageCodecDXT* ms_pInstance;
	
	public:
		static void	Startup();
		static void	Shutdown();

    public:
		virtual String GetCodecType() const;

		virtual String MagicNumberToFileExt(const char* szMagicNum, size_t nMaxbytes) const;
		
		virtual FFileMemory* Code(FFileMemory* pInput, FCodecData* pCodecData) const;
		virtual bool CodeToFile(FFileMemory* pInput, const String& outFilePath, FCodecData* pCodecData) const;
		
		virtual FDecodeResult Decode(FFileMemory* pInput) const;

	public:
		virtual FDecodeResult Decode(const String& strPath, bool isRGBA) const;

    public:
		/** Combine four DXT textures to one, these four dxt textures will be combined like that:
		tex0 | tex1
		----------- 
		tex2 | tex3
		@param
		mipmaps Indicate if to calculate the mipmaps of these textures, default value is 0, means to calculate mipmaps.*/ 
		FDecodeResult CombineFourTexture(FFileIO* stream0, FFileIO* stream1, FFileIO* stream2, FFileIO* stream3, uint16 mipmaps = 0);
		
		/** Combine two DXT textures horizontally :
		tex0 | tex1*/ 
		FDecodeResult CombineTwoTextureHorizontal(FFileIO* stream0, FFileIO* stream1, size_t mipmaps = 0);
		
		/** Combine two DXT textures vertically :
		tex0
		----
		tex1*/ 
		FDecodeResult CombineTwoTextureVertical(FFileIO* stream0, FFileIO* stream1, size_t mipmaps = 0);
		
		// Combine textures in same size
		FDecodeResult CombineTexturesInSameSize(FFileIOPtrVector stream, uint16 mipmaps = 0);
		
		// Check if these DXT textures can combine
		bool CheckCombineTextures(FFileIO* stream, uint16 num, size_t* height, size_t* width, size_t* pixelblocksize, size_t* mipmap);

    private:
		FPixelFormatType convertFourCCFormat(uint32 fourcc) const;
		FPixelFormatType convertPixelFormat(uint32 rgbBits, uint32 rMask, uint32 gMask, uint32 bMask, uint32 aMask) const;

		uint32 convertFormatToFourCC(FPixelFormatType format) const;

		/// Unpack DXT colors,alpha into array of 16 color values
		void unpackDXTColor(FPixelFormatType ePixelFormat, const DXTColorBlock& block, FColor* pCol) const;
		void unpackDXTAlpha(const DXTExplicitAlphaBlock& block, FColor* pCol) const;
		void unpackDXTAlpha(const DXTInterpolatedAlphaBlock& block, FColor* pCol) const;
		
	private:
		int	_DXTColorTypeToSquishType(FPixelFormatType format) const;
    };
    

}; //LostPeterEngine

#endif