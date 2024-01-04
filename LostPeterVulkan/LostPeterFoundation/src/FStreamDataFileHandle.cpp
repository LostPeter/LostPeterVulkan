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

#include "../include/FStreamDataFileHandle.h"

namespace LostPeterFoundation
{
    FStreamDataFileHandle::FStreamDataFileHandle(FILE* pFileHandle, int eStreamAccess /*= F_StreamAccess_Read*/)
		: FStreamData(eStreamAccess)
		, m_pFileHandle(pFileHandle)
	{
		fseek(m_pFileHandle, 0, SEEK_END);
		m_nSize = ftell(m_pFileHandle);
		fseek(m_pFileHandle, 0, SEEK_SET);
	}

	FStreamDataFileHandle::FStreamDataFileHandle(const String& strName, FILE* pFileHandle, int eStreamAccess /*= F_StreamAccess_Read*/)
		: FStreamData(strName,eStreamAccess)
		, m_pFileHandle(pFileHandle)
	{
		fseek(m_pFileHandle, 0, SEEK_END);
		m_nSize = ftell(m_pFileHandle);
		fseek(m_pFileHandle, 0, SEEK_SET);
	}

	FStreamDataFileHandle::~FStreamDataFileHandle()
	{
		Close();
	}

	size_t FStreamDataFileHandle::Read(void* pBuffer, size_t nCount)
	{
		return fread(pBuffer, 1, nCount, m_pFileHandle);
	}

	size_t FStreamDataFileHandle::Write(const void* pBuffer, size_t nCount)
	{
		if (!IsWriteable())
			return 0;
		else
			return fwrite(pBuffer, 1, nCount, m_pFileHandle);
	}

	void FStreamDataFileHandle::Skip(long nCount)
	{
		fseek(m_pFileHandle, nCount, SEEK_CUR);
	}

	void FStreamDataFileHandle::Seek(size_t nPos)
	{
		fseek(m_pFileHandle,static_cast<long>(nPos), SEEK_SET);
	}

	size_t FStreamDataFileHandle::Tell() const
	{
		return ftell(m_pFileHandle);
	}

	bool FStreamDataFileHandle::Eof() const
	{
		return feof(m_pFileHandle) != 0;
	}

	void FStreamDataFileHandle::Close()
	{
		if(m_pFileHandle)
		{
			fclose(m_pFileHandle);
			m_pFileHandle = 0;
		}
	}

}; //LostPeterFoundation