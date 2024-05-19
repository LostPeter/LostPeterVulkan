/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_TEXTURE_VIEW_H_
#define _RHI_DUMMY_TEXTURE_VIEW_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyTextureView : public RHITextureView
    {
    public:
        RHIDummyTextureView(RHIDummyTexture* pDummyTexture, const RHITextureViewCreateInfo& createInfo);
        virtual ~RHIDummyTextureView();

    public:
    protected:
        RHIDummyTexture* m_pDummyTexture;

    public:
        F_FORCEINLINE RHIDummyTexture* GetDummyTexture() { return m_pDummyTexture; }


    public:
        virtual void Destroy();

    };

}; //LostPeterPluginRHIDummy

#endif