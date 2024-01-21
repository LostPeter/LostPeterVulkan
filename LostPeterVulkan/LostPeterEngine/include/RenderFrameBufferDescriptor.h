/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_FRAME_BUFFER_DESCRIPTOR_H_
#define _RENDER_FRAME_BUFFER_DESCRIPTOR_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderFrameBufferDescriptor : public Base
                                                   , public FNonCopyable
    {
    public:
        RenderFrameBufferDescriptor(const String& nameRenderFrameBufferDescriptor);
        virtual ~RenderFrameBufferDescriptor();

    public:
    protected:
        TexturePtrVector m_aTextureSwapChain;
        TexturePtrVector m_aTextureColor;
        TexturePtrVector m_aTextureDepth;

    public:
        virtual void Destroy();
        

    public:
    protected:
        virtual bool createTextureColor() = 0;
        virtual bool createTextureDepth() = 0;

        virtual bool createRenderPass() = 0;
        virtual bool createRenderFrameBuffer() = 0;
    };

}; //LostPeterEngine

#endif