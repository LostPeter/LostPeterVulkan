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

#ifndef _IMAGE_CODEC_STB_H_
#define _IMAGE_CODEC_STB_H_

#include "ImageCodecSTBDefine.h"

namespace LostPeterPluginImageCodecSTB
{
    class ImageCodecSTB : public ImageCodec
    {
    public:
        ImageCodecSTB(const String& type);
        virtual ~ImageCodecSTB();

    private:
        String m_strImageType;

        typedef std::list<ImageCodec*> RegisteredImageCodecList;
        static RegisteredImageCodecList ms_listImageCodec;

    public:
        static void	Startup();
        static void	Shutdown();

    public:
        virtual String GetCodecType() const;

        virtual String MagicNumberToFileExt(const char* szMagicNum, size_t nMaxbytes) const;

        virtual FFileMemory* Code(FFileMemory* pInput, FCodecData* pCodecData) const;

        virtual bool CodeToFile(FFileMemory* pInput, const String& outFilePath, FCodecData* pCodecData) const;

        virtual FDecodeResult Decode(FFileMemory* pInput) const;
    };

}; //LostPeterPluginImageCodecSTB

#endif