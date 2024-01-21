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
        TexturePtrVector m_aTextureColor;
        TexturePtrVector m_aTextureDepth;
        TexturePtrVector m_aTextureSwapChain;

    public:
        F_FORCEINLINE const TexturePtrVector& GetTextureColor() const { return m_aTextureColor; }
        F_FORCEINLINE const TexturePtrVector& GetTextureDepth() const { return m_aTextureDepth; }
        F_FORCEINLINE const TexturePtrVector& GetTextureSwapChain() const { return m_aTextureSwapChain; }

    public:
        virtual void Destroy();
        virtual bool Init(FTextureType eTexture,
                          FPixelFormatType ePixelFormatColor,
                          FPixelFormatType ePixelFormatDepth,
                          FMSAASampleCountType eMSAASampleCount,
                          bool bHasImGUI) = 0;
        

    public:



    protected:
        virtual bool createTextureColor(FTextureType eTexture,
                                        FPixelFormatType ePixelFormatColor, 
                                        FMSAASampleCountType eMSAASampleCount) = 0;
        virtual bool createTextureDepth(FTextureType eTexture,
                                        FPixelFormatType ePixelFormatDepth, 
                                        FMSAASampleCountType eMSAASampleCount) = 0;

        virtual bool createRenderPass() = 0;
        virtual bool createRenderFrameBuffer() = 0;
    };

}; //LostPeterEngine

#endif