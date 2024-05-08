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

#include "../include/FUtilFile.h"

namespace LostPeterFoundation
{
#if F_PLATFORM != F_PLATFORM_WINDOW

    struct _find_search_t
	{
		char *pattern;
		char *curfn;
		char *directory;
		int dirlen;
		DIR *dirfd;
	};
			
	intptr_t _findfirst(const char *pattern, struct _finddata_t *data)
	{
		_find_search_t *fs = new _find_search_t;
		fs->curfn = NULL;
		fs->pattern = NULL;

		// Separate the mask from directory name
		const char *mask = strrchr (pattern, '/');
		if (mask)
		{
			fs->dirlen = static_cast<int>(mask - pattern);
			mask++;
			fs->directory = (char *)malloc (fs->dirlen + 1);
			memcpy (fs->directory, pattern, fs->dirlen);
			fs->directory [fs->dirlen] = 0;
		}
		else
		{
			mask = pattern;
			fs->directory = strdup (".");
			fs->dirlen = 1;
		}

		fs->dirfd = opendir (fs->directory);
		if (!fs->dirfd)
		{
			_findclose ((intptr_t)fs);
			return -1;
		}

		/* Hack for "*.*" -> "*' from DOS/Windows */
		if (strcmp (mask, "*.*") == 0)
			mask += 2;
		fs->pattern = strdup (mask);

		/* Get the first entry */
		if (_findnext ((intptr_t)fs, data) < 0)
		{
			_findclose ((intptr_t)fs);
			return -1;
		}

		return (intptr_t)fs;
	}

	int _findnext(intptr_t id, struct _finddata_t *data)
	{
		_find_search_t *fs = reinterpret_cast<_find_search_t *>(id);

		/* Loop until we run out of entries or find the next one */
		dirent *entry;
		for (;;)
		{
			if (!(entry = readdir (fs->dirfd)))
				return -1;

			/* See if the filename matches our pattern */
			if (fnmatch (fs->pattern, entry->d_name, 0) == 0)
				break;
		}

		if (fs->curfn)
			free (fs->curfn);
		data->name = fs->curfn = strdup (entry->d_name);

		size_t namelen = strlen (entry->d_name);
		char *xfn = new char [fs->dirlen + 1 + namelen + 1];
		sprintf (xfn, "%s/%s", fs->directory, entry->d_name);

		/* stat the file to get if it's a subdir and to find its length */
		struct stat stat_buf;
		if (stat (xfn, &stat_buf))
		{
			// Hmm strange, imitate a zero-length file then
			data->attrib = _A_NORMAL;
			data->size = 0;
		}
		else
		{
			if (S_ISDIR(stat_buf.st_mode))
				data->attrib = _A_SUBDIR;
			else
				/* Default type to a normal file */
				data->attrib = _A_NORMAL;

			data->size = (unsigned long)stat_buf.st_size;
		}

		delete [] xfn;

		/* Files starting with a dot are hidden files in Unix */
		if (data->name [0] == '.')
			data->attrib |= _A_HIDDEN;

		return 0;
	}

	int _findclose(intptr_t id)
	{
		int ret;
		_find_search_t *fs = reinterpret_cast<_find_search_t *>(id);
		
		ret = fs->dirfd ? closedir (fs->dirfd) : 0;
		free (fs->pattern);
		free (fs->directory);
		if (fs->curfn)
			free (fs->curfn);
		delete fs;

		return ret;
	}

#endif
}; //LostPeterFoundation