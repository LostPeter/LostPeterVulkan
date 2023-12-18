/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-08-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_018_object.h"


Vulkan_018_Object::Vulkan_018_Object(const String& nameSample, int width, int height)
    : Sample(nameSample, width, height)
{
    m_cfg_bIsWindowSceneUsed = true;
    m_cfg_bIsWindowGameUsed = false;
}