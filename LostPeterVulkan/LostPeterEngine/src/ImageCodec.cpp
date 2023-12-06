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

#include "../include/ImageCodec.h"

namespace LostPeterEngine
{
    ImageCodec::ImageData::ImageData()
        : nHeight(0)
        , nWidth(0)
        , nDepth(1)
        , nSize(0)
        , nNumMipmaps(0)
        , nFlags(0)
        , ePixelFormat(F_PixelFormat_Unknown)
    {

    }

    ImageCodec::ImageData::~ImageData()
    {

    }

    String ImageCodec::ImageData::GetDataType() const	
    {
        return "ImageData";
    }


    ImageCodec::ImageCodec()
    {

    }

	ImageCodec::~ImageCodec()
    {

    }

    String ImageCodec::GetDataType() const
    {
        return "ImageData";
    }

}; //LostPeterEngine