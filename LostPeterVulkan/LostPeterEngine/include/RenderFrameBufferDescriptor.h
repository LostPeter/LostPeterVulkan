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

        FTextureType m_eTexture;
        FPixelFormatType m_ePixelFormatColor;
        FPixelFormatType m_ePixelFormatDepth;
        FPixelFormatType m_ePixelFormatSwapChian;
        FMSAASampleCountType m_eMSAASampleCount;
        bool m_bIsUseImGUI;

    public:
        F_FORCEINLINE const TexturePtrVector& GetTextureColor() const { return m_aTextureColor; }
        F_FORCEINLINE const TexturePtrVector& GetTextureDepth() const { return m_aTextureDepth; }
        F_FORCEINLINE const TexturePtrVector& GetTextureSwapChain() const { return m_aTextureSwapChain; }

        F_FORCEINLINE FTextureType GetTextureType() const { return m_eTexture; }
        F_FORCEINLINE FPixelFormatType GetPixelFormatColor() const { return m_ePixelFormatColor; }
        F_FORCEINLINE FPixelFormatType GetPixelFormatDepth() const { return m_ePixelFormatDepth; }
        F_FORCEINLINE FPixelFormatType GetPixelFormatSwapChian() const { return m_ePixelFormatSwapChian; }
        F_FORCEINLINE FMSAASampleCountType GetMSAASampleCount() const { return m_eMSAASampleCount; }

        F_FORCEINLINE bool HasMSAASampleCount() const { return m_eMSAASampleCount != F_MSAASampleCount_1_Bit; }
        F_FORCEINLINE bool GetIsUseImGUI() const { return m_bIsUseImGUI; }

    public:
        virtual void Destroy();
        virtual bool Init(FTextureType eTexture,
                          FPixelFormatType ePixelFormatColor,
                          FPixelFormatType ePixelFormatDepth,
                          FPixelFormatType ePixelFormatSwapChian,
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