/****************************************************************************
* LostPeterPluginImageCodecSTB - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ImageCodecSTBInclude.h"
#include "../include/ImageCodecSTB.h"


#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include "stb_image_write.h"


namespace LostPeterPluginImageCodecSTB
{
    ImageCodecSTB::RegisteredImageCodecList ImageCodecSTB::ms_listImageCodec;

    ImageCodecSTB::ImageCodecSTB(const String& type)
        : m_strImageType(type)
    {

    }

    ImageCodecSTB::~ImageCodecSTB()
    {

    }

    void ImageCodecSTB::Startup()
    {
        stbi_convert_iphone_png_to_rgb(1);
        stbi_set_unpremultiply_on_load(1);
        F_LogInfo("ImageCodecSTB::Startup: STB_Image codec registering !");
        
        //Register Codecs
        String exts = "jpeg,jpg,png,bmp,psd,tga,gif,pic,ppm,pgm,hdr";
        StringVector extsVector = FUtilString::Split(exts, ",");
        for (StringVector::iterator it = extsVector.begin(); 
             it != extsVector.end(); ++it)
        {
            ImageCodec* pCodec = new ImageCodecSTB(*it);
            ms_listImageCodec.push_back(pCodec);
            FCodecManager::RegisterCodec(pCodec);
        }
        F_LogInfo("ImageCodecSTB::Startup: Supported formats: [%s] !", exts.c_str());
    }

    void ImageCodecSTB::Shutdown()
    {
        //UnRegister Codecs
        for (RegisteredImageCodecList::iterator it = ms_listImageCodec.begin();
             it != ms_listImageCodec.end(); ++it)
        {
            FCodecManager::UnRegisterCodec(*it);
            F_DELETE(*it)
        }
        ms_listImageCodec.clear();
        F_LogInfo("ImageCodecSTB::Shutdown: STB_Image codec unregistering !");
    }

    String ImageCodecSTB::GetCodecType() const
    {
        return m_strImageType;
    }

    String ImageCodecSTB::MagicNumberToFileExt(const char* szMagicNum, size_t nMaxbytes) const
    {
        return FUtilString::BLANK;
    }

    static const String s_extWrite = "png";
    FFileMemory* ImageCodecSTB::Code(FFileMemory* pInput, FCodecData* pCodecData) const
    {
        if (m_strImageType != s_extWrite)
        {
            F_LogError("ImageCodecSTB::Code: Currently only encoding to PNG supported !");
            return nullptr;
        }

        ImageData* pImgData = static_cast<ImageData*>(pCodecData);
        FPixelFormatType format = pImgData->ePixelFormat;
        uint8* pInputData = pInput->GetBuffer();

        // Convert image data to ABGR format for STBI (unless it's already compatible)
        uint8* pTempData = 0;
        if (format != F_PixelFormat_BYTE_A8B8G8R8_UNORM && 
            format != F_PixelFormat_BYTE_B8G8R8_UNORM && 
            format != F_PixelFormat_BYTE_AL_UNORM && 
            format != F_PixelFormat_BYTE_L8_UNORM && 
            format != F_PixelFormat_BYTE_R8_UNORM)
        {   
            format = F_PixelFormat_BYTE_A8B8G8R8_UNORM;
            size_t tempDataSize = pImgData->nWidth * pImgData->nHeight * pImgData->nDepth * FPixelFormat::GetPixelFormatElemBytes(format);
            pTempData = new uint8[tempDataSize];
            FPixelBox pbIn(pImgData->nWidth, pImgData->nHeight, pImgData->nDepth, pImgData->ePixelFormat, pInputData);
            FPixelBox pbOut(pImgData->nWidth, pImgData->nHeight, pImgData->nDepth, format, pTempData);
            FPixelFormat::BulkPixelConversion(pbIn, pbOut);

            pInputData = pTempData;
        }

        // Save to PNG
        int channels = (int)FPixelFormat::GetPixelFormatComponentCount(format);
        int stride = pImgData->nWidth * (int)FPixelFormat::GetPixelFormatElemBytes(format);
        int len;
        uint8* pBuf = stbi_write_png_to_mem(pInputData, stride, pImgData->nWidth, pImgData->nHeight, channels, &len);
        FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory(pBuf, pImgData->nSize);

        if (pTempData)
        {
            F_DELETE_T(pTempData)
        }

        if (!pBuf) 
        {
            F_LogError("ImageCodecSTB::Code: Error code image: [%s] !", stbi_failure_reason());
            F_Assert(false && "ImageCodecSTB::Code: Error code image !")
        }

        return pFM;
    }

    bool ImageCodecSTB::CodeToFile(FFileMemory* pInput, const String& outFilePath, FCodecData* pCodecData) const
    {
        FFileMemory* pFile = Code(pInput, pCodecData);
        std::ofstream file(outFilePath.c_str(), std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            F_LogError("ImageCodecSTB::CodeToFile: Could not open file: [%s] !", outFilePath.c_str());
            F_Assert(false && "ImageCodecSTB::CodeToFile: Could not open file !")
            return false;
        }
        file.write((char*)pFile->GetBuffer(), pFile->Size());
        FFileManager::GetSingleton().DeleteFileMemory(pFile);

        return true;
    }

    ImageCodecSTB::FDecodeResult ImageCodecSTB::Decode(FFileMemory* pInput) const
    {
        uint8* pData = pInput->GetBuffer();
        int width, height, components;
        stbi_uc* pixelData = stbi_load_from_memory((const uint8*)pData,
                static_cast<int>(pInput->Size()), &width, &height, &components, 0);

        if (!pixelData)
        {
            F_LogError("ImageCodecSTB::Decode: Error decode image: [%s] !", stbi_failure_reason());
            F_Assert(false && "ImageCodecSTB::Decode: Error decode image !")
        }

        ImageData* pImageData = new ImageData();
        pImageData->nDepth = 1; // only 2D formats handled by this codec
        pImageData->nWidth = width;
        pImageData->nHeight = height;
        pImageData->nNumMipmaps = 0; // no mipmaps in non-DDS 
        pImageData->nFlags = 0;

        switch (components)
        {
            case 1:
                pImageData->ePixelFormat = F_PixelFormat_BYTE_L8_UNORM;
                break;
            case 2:
                pImageData->ePixelFormat = F_PixelFormat_BYTE_AL_UNORM;
                break;
            case 3:
                pImageData->ePixelFormat = F_PixelFormat_BYTE_R8G8B8_UNORM;
                break;
            case 4:
                pImageData->ePixelFormat = F_PixelFormat_BYTE_A8R8G8B8_UNORM;
                break;
            default:
                stbi_image_free(pixelData);
                F_Assert(false && "ImageCodecSTB::Decode: Unknown or unsupported image format !")
                break;
        }
        
        size_t dstPitch = pImageData->nWidth * FPixelFormat::GetPixelFormatElemBytes(pImageData->ePixelFormat);
        pImageData->nSize = (int32)(dstPitch * pImageData->nHeight);
        FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory(pixelData, pImageData->nSize);
        
        FDecodeResult ret;
        ret.first = pFM;
        ret.second = pImageData;
        return ret;
    }

}; //LostPeterPluginImageCodecSTB