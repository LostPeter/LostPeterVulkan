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

#ifndef _RESOURCE_GROUP_LISTENER_H_
#define _RESOURCE_GROUP_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ResourceGroupListener
    {
    public:
        ResourceGroupListener();
        virtual ~ResourceGroupListener();

    public:
    protected:

        
    public:
        //Script
		virtual void ResourceGroupScriptingStarted(const String& strGroupName, size_t nScriptCount) = 0;	
		virtual void ScriptParseStarted(const String& scriptName, bool& bIsSkipThisScript)	= 0;	
		virtual void ScriptParseEnded(const String& scriptName, bool bSkipped) = 0;	
		virtual void ResourceGroupScriptingEnded(const String& strGroupName) = 0;	

		//Resource Load
		virtual void ResourceGroupLoadStarted(const String& strGroupName, size_t nResourceCount) = 0;	
		virtual void ResourceLoadStarted(const Resource* pResource) = 0;	
		virtual void ResourceLoadEnded() = 0;	
		virtual void WorldGeometryStageStarted(const String& strDescription) = 0;
		virtual void WorldGeometryStageEnded() = 0;	
		virtual void ResourceGroupLoadEnded(const String& strGroupName) = 0;	

		//Resource Prepare
		virtual void ResourceGroupPrepareStarted(const String& strGroupName, size_t nResourceCount) {}		
		virtual void ResourcePrepareStarted(const Resource* pResource) {}		
		virtual void ResourcePrepareEnded() {}		
		virtual void WorldGeometryStagePrepareStarted(const String& strDescription) {}		
		virtual void WorldGeometryStagePrepareEnded() {}		
		virtual void ResourceGroupPrepareEnded(const String& strGroupName) {}		
    };

}; //LostPeterEngine

#endif