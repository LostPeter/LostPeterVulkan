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

#include "../include/RenderFrameBufferDescriptor.h"


namespace LostPeterEngine
{
    RenderFrameBufferDescriptor::RenderFrameBufferDescriptor(const String& nameRenderFrameBufferDescriptor)
        : Base(nameRenderFrameBufferDescriptor)
        , m_eTexture(F_Texture_2D)
        , m_ePixelFormatColor(F_PixelFormat_BYTE_A8R8G8B8_SRGB)
        , m_ePixelFormatDepth(F_PixelFormat_DEPTH_D32_SFLOAT)
        , m_ePixelFormatSwapChian(F_PixelFormat_BYTE_A8R8G8B8_SRGB)
        , m_eMSAASampleCount(F_MSAASampleCount_1_Bit)
        , m_bIsUseImGUI(false)
    {

    }

    RenderFrameBufferDescriptor::~RenderFrameBufferDescriptor()
    {

    }

    void RenderFrameBufferDescriptor::Destroy()
    {
        
    }

}; //LostPeterEngine