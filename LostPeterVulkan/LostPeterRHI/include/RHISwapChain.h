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

#ifndef _RHI_SWAP_CHAIN_H_
#define _RHI_SWAP_CHAIN_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHISwapChain : public RHIObject
    {
    public:
        RHISwapChain(RHIDevice* pDevice, const RHISwapChainCreateInfo& createInfo);
        virtual ~RHISwapChain();

    public:
    protected:
        

    public:
        virtual RHITexture* GetTexture(uint8 nIndex) = 0;
        virtual uint8 AcquireBackTexture() = 0;
        virtual void Present() = 0;
    };

}; //LostPeterRHI

#endif