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

#ifndef _RHI_DUMMY_SWAP_CHAIN_H_
#define _RHI_DUMMY_SWAP_CHAIN_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummySwapChain : public RHISwapChain
    {
    public:
        RHIDummySwapChain(const RHISwapChainCreateInfo& createInfo);
        virtual ~RHIDummySwapChain();

    public:
    protected:
        RHIDummyTexturePtrVector m_aDummyTextures;
        bool m_bIsPingPong;

    public:
        virtual void Destroy();

        virtual RHITexture* GetTexture(uint8 nIndex);
        virtual uint8 AcquireBackTexture();
        virtual void Present();
    };

}; //LostPeterPluginRHIDummy

#endif