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

#ifndef _RESOURCE_LOADING_LISTENER_H_
#define _RESOURCE_LOADING_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ResourceLoadingListener
    {
    public:
        ResourceLoadingListener();
        virtual ~ResourceLoadingListener();

    public:
    protected:

        
    public:
        virtual FStreamData* ResourceLoading(const String& strName,const String& strGroupName, Resource* pResource) = 0;
        virtual void ResourceStreamOpened(const String& strName,const String& strGroupName, Resource* pResource, FStreamData* pStream) = 0;
        virtual bool ResourceCollision(Resource* pResource, ResourceManager* pResourceManager) = 0;        
    };

}; //LostPeterEngine

#endif