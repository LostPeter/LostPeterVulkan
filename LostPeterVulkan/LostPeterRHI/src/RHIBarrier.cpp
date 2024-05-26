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

#include "../include/RHIBarrier.h"

namespace LostPeterRHI
{
    RHIBarrier RHIBarrier::Transition(RHIBuffer* pBuffer, RHIBufferStateType eStateBefore, RHIBufferStateType eStateAfter)
    {
        RHIBarrier barrier {};
        barrier.eResource = RHIResourceType::RHI_Resource_Buffer;
        barrier.transBuffer.pBuffer = pBuffer;
        barrier.transBuffer.eStateBefore = eStateBefore;
        barrier.transBuffer.eStateAfter = eStateAfter;
        return barrier;
    }
    RHIBarrier RHIBarrier::Transition(RHITexture* pTexture, RHITextureStateType eStateBefore, RHITextureStateType eStateAfter)
    {
        RHIBarrier barrier {};
        barrier.eResource = RHIResourceType::RHI_Resource_Texture;
        barrier.transTexture.pTexture = pTexture;
        barrier.transTexture.eStateBefore = eStateBefore;
        barrier.transTexture.eStateAfter = eStateAfter;
        return barrier;
    }

    RHIBarrier::RHIBarrier()
    {

    }

    RHIBarrier::~RHIBarrier()
    {
        
    }
    

}; //LostPeterRHI