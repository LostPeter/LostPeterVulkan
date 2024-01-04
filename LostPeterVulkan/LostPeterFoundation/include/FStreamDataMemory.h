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

#ifndef _F_STREAM_DATA_MEMORY_H_
#define _F_STREAM_DATA_MEMORY_H_

#include "FStreamData.h"

namespace LostPeterFoundation
{
	class foundationExport FStreamDataMemory : public FStreamData
	{
	public:
		FStreamDataMemory(void* pMemory, size_t nSize, bool bIsFreeOnClose = false, bool bIsReadOnly = false);
		FStreamDataMemory(const String& strName, void* pMemory, size_t nSize, bool bIsFreeOnClose = false, bool bIsReadOnly = false);
		FStreamDataMemory(FStreamData& streamSrc, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		FStreamDataMemory(FStreamData* pStreamSrc, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		FStreamDataMemory(const String& strName, FStreamData& streamSrc, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		FStreamDataMemory(const String& strName, FStreamData* pStreamSrc, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		FStreamDataMemory(size_t nSize, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		FStreamDataMemory(const String& strName, size_t nSize, bool bIsFreeOnClose = true, bool bIsReadOnly = false);
		virtual ~FStreamDataMemory();

	public:
	protected:
		uint8* m_pData;
		uint8* m_pPos;
		uint8* m_pEnd;
		bool m_bIsFreeOnClose;			

	public:
		F_FORCEINLINE uint8* GetPtr() { return m_pData; }
		F_FORCEINLINE uint8* GetCurrentPtr() { return m_pPos; }

	public:
		virtual size_t Read(void* pBuffer, size_t nCount);
		virtual size_t ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim = "\n");
		virtual size_t Write(const void* pBuffer, size_t nCount);
		virtual size_t SkipLine(const String& strDelim = "\n");
		virtual void Skip(long nCount);
		virtual void Seek(size_t nPos);
		virtual size_t Tell() const;
		virtual bool Eof() const;
		virtual void Close();

		virtual void SetIsFreeOnClose(bool bIsFreeOnClose) { m_bIsFreeOnClose = bIsFreeOnClose; }
	};

}; //LostPeterFoundation

#endif