/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_BARRIER_H_
#define _RHI_BARRIER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIBarrier
    {
    public:
        RHIBarrier();
        virtual ~RHIBarrier();

    public:
        struct RHIBufferTransition 
        {
            RHIBuffer* pBuffer;
            RHIBufferStateType eStateBefore;
            RHIBufferStateType eStateAfter;
        };

        struct RHITextureTransition 
        {
            RHITexture* pTexture;
            RHITextureStateType eStateBefore;
            RHITextureStateType eStateAfter;
        };

        RHIResourceType eResource;
        union 
        {
            RHIBufferTransition transBuffer;
            RHITextureTransition transTexture;
        };

    protected:


    public:
        static RHIBarrier Transition(RHIBuffer* pBuffer, RHIBufferStateType eStateBefore, RHIBufferStateType eStateAfter);
        static RHIBarrier Transition(RHITexture* pTexture, RHITextureStateType eStateBefore, RHITextureStateType eStateAfter);

    };
    
}; //LostPeterRHI

#endif