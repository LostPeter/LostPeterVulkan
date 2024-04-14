/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#if F_PLATFORM == F_PLATFORM_MAC || F_PLATFORM == F_PLATFORM_LINUX || F_PLATFORM == F_PLATFORM_ANDROID || F_PLATFORM == F_PLATFORM_iOS

#ifndef _F_UTIL_FILE_H_
#define _F_UTIL_FILE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	struct _finddata_t
	{
		char *name;
		int attrib;
		unsigned long size;
	};

	#define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
	#define _A_RDONLY 0x01  /* Read only file */
	#define _A_HIDDEN 0x02  /* Hidden file */
	#define _A_SYSTEM 0x04  /* System file */
	#define _A_ARCH   0x20  /* Archive file */
	#define _A_SUBDIR 0x10  /* Subdirectory */

	intptr_t _findfirst(const char *pattern, struct _finddata_t *data);
	int _findnext(intptr_t id, struct _finddata_t *data);
	int _findclose(intptr_t id);

}; //LostPeterFoundation

#endif

#endif