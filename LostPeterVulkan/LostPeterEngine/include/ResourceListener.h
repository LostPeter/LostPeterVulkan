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

#ifndef _RESOURCE_LISTENER_H_
#define _RESOURCE_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ResourceListener
    {
    public:
        ResourceListener();
        virtual ~ResourceListener();

    public:
    protected:


    public:


    public:
        virtual void PreparingComplete(Resource* pResource);
        virtual void LoadingComplete(Resource* pResource);
        virtual void UnloadingComplete(Resource* pResource);
    };

}; //LostPeterEngine

#endif