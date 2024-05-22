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

#ifndef _RHI_TEXTURE_H_
#define _RHI_TEXTURE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHITexture : public RHIObject
    {
    public:
        RHITexture(RHIDevice* pDevice, const RHITextureCreateInfo& createInfo);
        virtual ~RHITexture();

    public:
    protected:
        RHITextureView* m_pTextureView;

    public:
        F_FORCEINLINE RHITextureView* GetTextureView() { return m_pTextureView; }

    public:
        virtual void DestroyTextureView() = 0;
        virtual RHITextureView* CreateTextureView(const RHITextureViewCreateInfo& createInfo) = 0;
    };

}; //LostPeterRHI

#endif