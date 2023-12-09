/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PLUGIN_H_
#define _F_PLUGIN_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FPlugin
	{
	public:
		FPlugin();
		virtual ~FPlugin();

	public:
		virtual const String& GetName() const = 0;
		virtual void Install() = 0;
		virtual void Uninstall() = 0;
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
	};

}; //LostPeterFoundation

#endif