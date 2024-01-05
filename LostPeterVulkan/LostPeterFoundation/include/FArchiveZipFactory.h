/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_ARCHIVE_ZIP_FACTORY_H_
#define _F_ARCHIVE_ZIP_FACTORY_H_

#include "FArchiveFactory.h"

namespace LostPeterFoundation
{
	class foundationExport FArchiveZipFactory : public FArchiveFactory
	{
	public:
		FArchiveZipFactory();
		virtual ~FArchiveZipFactory();

	public:
		static String ms_strFactoryType;

	public:
		virtual const String& GetType() const;
		virtual FArchive* CreateInstance(const String& strName);
		virtual void DestroyInstance(FArchive* pInstance);
	};

}; //LostPeterFoundation

#endif