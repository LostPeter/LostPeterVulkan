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

#include "../include/RHICommandEncoder.h"

namespace LostPeterRHI
{
    //////////////////////// RHIComputePassCommandEncoder ////////////////////////
    RHIComputePassCommandEncoder::RHIComputePassCommandEncoder(RHICommandEncoder* pCommandEncoder)
        : m_pCommandEncoder(pCommandEncoder)
    {
        F_Assert(m_pCommandEncoder && "RHIComputePassCommandEncoder::RHIComputePassCommandEncoder")
    }   

    RHIComputePassCommandEncoder::~RHIComputePassCommandEncoder()
    {
        
    }

    //////////////////////// RHIGraphicsPassCommandEncoder ///////////////////////
    RHIGraphicsPassCommandEncoder::RHIGraphicsPassCommandEncoder(RHICommandEncoder* pCommandEncoder)
        : m_pCommandEncoder(pCommandEncoder)
    {
        F_Assert(m_pCommandEncoder && "RHIGraphicsPassCommandEncoder::RHIGraphicsPassCommandEncoder")
    }

    RHIGraphicsPassCommandEncoder::~RHIGraphicsPassCommandEncoder()
    {
        
    }

    //////////////////////// RHICommandEncoder ///////////////////////////////////
    RHICommandEncoder::RHICommandEncoder(RHICommandBuffer* pCommandBuffer)
        : m_pCommandBuffer(pCommandBuffer)
    {
        F_Assert(m_pCommandBuffer && "RHICommandEncoder::RHICommandEncoder")
    }

    RHICommandEncoder::~RHICommandEncoder()
    {
        
    }
    

}; //LostPeterRHI