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

#ifndef _F_ARCHIVE_ZIP_H_
#define _F_ARCHIVE_ZIP_H_

#include "FArchive.h"

namespace LostPeterFoundation
{
    class foundationExport FArchiveZip : public FArchive
    {
    public:
		FArchiveZip(const String& strName, const String& strType);
		virtual ~FArchiveZip();

    public:


    };

}; //LostPeterFoundation

#endif