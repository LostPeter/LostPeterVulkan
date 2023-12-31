/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ResourceManager.h"
#include "../include/Resource.h"

namespace LostPeterEngine
{
    ResourceManager::ResourceManager(const String& strName, EResourceType eResource)
        : Base(strName)
        , m_eResource(eResource)
    {

    }

    ResourceManager::~ResourceManager()
    {
        
    }   

    void ResourceManager::Destroy()
    {
       
    }


}; //LostPeterEngine