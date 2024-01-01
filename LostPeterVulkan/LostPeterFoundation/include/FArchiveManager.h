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

#ifndef _F_ARCHIVE_MANAGER_H_
#define _F_ARCHIVE_MANAGER_H_

#include "FSingleton.h"
#include "../include/FArchiveFactory.h"

namespace LostPeterFoundation
{
	class foundationExport FArchiveManager : public FSingleton<FArchiveManager>
	{
	public:
		FArchiveManager();
		virtual ~FArchiveManager();

	public:
	protected:
		FArchiveFactoryPtrMap m_mapArchiveFactory;		
		FArchivePtrMap m_mapArchive;

	public:
		static FArchiveManager& GetSingleton();
		static FArchiveManager* GetSingletonPtr();

	public:
		void Destroy(); 

		FArchivePtrMapIterator GetArchivePtrMapIterator();
		void AddArchiveFactory(FArchiveFactory* pArchiveFactory);

	public:
		FArchive* Load(const String& strFileName, const String& strArchiveType);
		void Unload(FArchive* pArchive);
		void Unload(const String& strFileName);
	};

}; //LostPeterFoundation

#endif