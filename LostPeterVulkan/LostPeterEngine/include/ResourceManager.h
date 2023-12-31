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

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ResourceManager : public Base
    {
    public:
        ResourceManager(const String& strName, EResourceType eResource);
        virtual ~ResourceManager();

    public:
    protected:
        EResourceType m_eResource;

    public:
        F_FORCEINLINE EResourceType GetResourceTType() const { return m_eResource; }

    public:
        virtual void Destroy();

    public:

    };

}; //LostPeterEngine

#endif