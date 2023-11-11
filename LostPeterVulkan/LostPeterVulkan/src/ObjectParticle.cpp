/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ObjectParticle.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    ObjectParticle::ObjectParticle(const String& nameParticle, Scene* pScene)
        : Object(nameParticle, pScene)
    {
        this->m_typeObject = Vulkan_Object_Particle;
    }
    ObjectParticle::~ObjectParticle()
    {

    }
    
}; //LostPeter