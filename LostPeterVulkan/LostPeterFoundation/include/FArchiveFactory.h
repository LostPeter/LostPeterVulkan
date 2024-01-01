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

#ifndef _F_ARCHIVE_FACTORY_H_
#define _F_ARCHIVE_FACTORY_H_

#include "FFactory.h"
#include "FArchive.h"

namespace LostPeterFoundation
{
	class foundationExport FArchiveFactory : public FFactory<FArchive>
	{
	public:
		FArchiveFactory();
		virtual ~FArchiveFactory();

	public:

	};

}; //LostPeterFoundation

#endif