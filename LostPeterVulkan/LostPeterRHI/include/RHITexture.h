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

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHITexture : public FNonCopyable
    {
    public:
        RHITexture(RHIDevice* pDevice, const RHITextureCreateInfo& createInfo);
        virtual ~RHITexture();

    public:
    protected:
        RHIDevice* m_pDevice;
        RHITextureView* m_pTextureView;

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }
        F_FORCEINLINE RHITextureView* GetTextureView() { return m_pTextureView; }

    public:
        virtual void Destroy() = 0;
        virtual void DestroyTextureView() = 0;
        virtual RHITextureView* CreateTextureView(const RHITextureViewCreateInfo& createInfo) = 0;
    };

}; //LostPeterRHI

#endif