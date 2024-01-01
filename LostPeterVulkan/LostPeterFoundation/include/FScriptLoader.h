/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_SCRIPT_LOADER_H_
#define _F_SCRIPT_LOADER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	class foundationExport FScriptLoader
	{
	public:
		FScriptLoader();
		virtual ~FScriptLoader();

	public:
		virtual const StringVector&	GetScriptPatterns() const = 0;
		virtual void ParseScript(FFileBase* pFileBase, const String& strGroupName) = 0;
		virtual float GetLoadingOrder() const = 0;
	};

}; //LostPeterFoundation

#endif