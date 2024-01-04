/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_STREAM_DATA_FILE_HANDLE_H_
#define _F_STREAM_DATA_FILE_HANDLE_H_

#include "FStreamData.h"

namespace LostPeterFoundation
{
	class foundationExport FStreamDataFileHandle : public FStreamData
	{
	public:
		FStreamDataFileHandle(FILE* pFileHandle, int eStreamAccess = F_StreamAccess_Read);
		FStreamDataFileHandle(const String& strName, FILE* pFileHandle, int eStreamAccess = F_StreamAccess_Read);
		virtual ~FStreamDataFileHandle();

	public:
	protected:
		FILE* m_pFileHandle;

	public:
		virtual size_t Read(void* pBuffer, size_t nCount);
		virtual size_t Write(const void* pBuffer, size_t nCount);
		virtual void Skip(long nCount);
		virtual void Seek(size_t nPos);
		virtual size_t Tell() const;
		virtual bool Eof() const;
		virtual void Close();
	};

}; //LostPeterFoundation

#endif