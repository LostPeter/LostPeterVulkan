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

#include "../include/RHITexture.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHITexture::RHITexture(RHIDevice* pDevice, const RHITextureCreateInfo& createInfo)
        : RHIObject(pDevice)
        , m_pTextureView(nullptr)
    {
        m_eObject = RHIObjectType::RHI_Object_Texture;
    }

    RHITexture::~RHITexture()
    {
        
    }
    

}; //LostPeterRHI