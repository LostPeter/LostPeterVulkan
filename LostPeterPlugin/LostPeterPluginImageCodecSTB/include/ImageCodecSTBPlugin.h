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

#ifndef _IMAGE_CODEC_STB_PLUGIN_H_
#define _IMAGE_CODEC_STB_PLUGIN_H_

#include "ImageCodecSTBDefine.h"

namespace LostPeter
{
    class ImageCodecSTBPlugin : public FPlugin
    {
    public:
        ImageCodecSTBPlugin();
        virtual ~ImageCodecSTBPlugin();

    public:
        virtual const String& GetName() const;
        virtual void Install();
        virtual void Uninstall();
        virtual void Initialize();
        virtual void Shutdown();
    };

}; //LostPeter

#endif