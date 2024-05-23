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

#ifndef _RHI_DUMMY_TEXTURE_H_
#define _RHI_DUMMY_TEXTURE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyTexture : public RHITexture
                                         , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyTexture(RHIDummyDevice* pDummyDevice, const RHITextureCreateInfo& createInfo);
    public:
        virtual ~RHIDummyTexture();

    public:
    protected:
        RHIDummyTextureView* m_pDummyTextureView;

    public:
        F_FORCEINLINE RHIDummyTextureView* GetDummyTextureView() { return m_pDummyTextureView; }


    public:
        virtual void Destroy();
        virtual void DestroyTextureView();
        virtual RHITextureView* CreateTextureView(const RHITextureViewCreateInfo& createInfo);
    };

}; //LostPeterPluginRHIDummy

#endif