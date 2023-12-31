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

#ifndef _RESOURCE_MANUAL_LOADER_H_
#define _RESOURCE_MANUAL_LOADER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ResourceManualLoader : public Base
    {
    public:
        ResourceManualLoader(const String& strName);
        virtual ~ResourceManualLoader();

    public:
    protected:


    public:


    public:
        virtual void PrepareResource(Resource* pResource);
        virtual void LoadResource(Resource* pResource) = 0;
    };

}; //LostPeterEngine

#endif