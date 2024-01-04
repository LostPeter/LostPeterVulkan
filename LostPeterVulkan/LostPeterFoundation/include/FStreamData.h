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

#ifndef _F_STREAM_DATA_H_
#define _F_STREAM_DATA_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	class foundationExport FStreamData
	{
	public:
		FStreamData(int eStreamAccess = F_StreamAccess_Read);
		FStreamData(const String& strName, int eStreamAccess = F_StreamAccess_Read);
		virtual ~FStreamData();

	public:
	protected:
		String m_strName;			
		size_t m_nSize;				
		int	m_eStreamAccess;				

	public:
		const String& GetName()	const { return m_strName; }
		int	GetAccessMode() const { return m_eStreamAccess; }
		virtual bool IsReadable() const	{ return (m_eStreamAccess & F_StreamAccess_Read) != 0; }
		virtual bool IsWriteable() const { return (m_eStreamAccess & F_StreamAccess_Write) != 0; }

		virtual size_t Read(void* pBuffer, size_t nCount) = 0;									
		virtual size_t ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim = "\n");			
		virtual size_t Write(const void* pBuffer, size_t nCount) { return 0; }
		virtual String GetLine(bool bIsTrimAfter = true);
		virtual String GetAsString();
		virtual size_t SkipLine(const String& strDelim = "\n");
		virtual void Skip(long nCount) = 0;
		virtual void Seek(size_t nPos) = 0;
		virtual size_t Tell() const	= 0;
		virtual bool Eof() const = 0;
		size_t Size() const { return m_nSize; }
		virtual void Close() = 0;

		template<typename T> 
		FStreamData& operator >>(T& val);
	};

}; //LostPeterFoundation

#endif