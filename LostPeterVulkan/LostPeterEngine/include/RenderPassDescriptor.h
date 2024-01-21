/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_DESCRIPTOR_H_
#define _RENDER_PASS_DESCRIPTOR_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPassDescriptor : public Base
                                            , public FNonCopyable
    {
    public:
        RenderPassDescriptor(const String& nameRenderPassDescriptor);
        virtual ~RenderPassDescriptor();

    public:
    protected:



    public:
        virtual void Destroy();
        virtual bool Init() = 0;

    
    public:
        virtual void SetClearColor(uint8 nIndex, const FColor& clearColor) = 0;
        virtual void SetClearColor(const FColor& clearColor) = 0;

        virtual void SetClearDepth(float clearDepth) = 0;
        virtual void SetClearStencil(uint32 clearStencil) = 0;


    };

}; //LostPeterEngine

#endif