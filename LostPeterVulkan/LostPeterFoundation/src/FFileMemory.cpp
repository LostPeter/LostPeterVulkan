/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FFileMemory.h"

namespace LostPeterFoundation
{
    FFileMemory::FFileMemory(uint8* pBuffer /* = 0 */, uint32 nSize /* = 0 */)
		: m_pBuffer(pBuffer)
		, m_nSize(nSize)
		, m_nEdge(nSize)
		, m_nPosition(0)
		, m_bFreeOnClose(pBuffer?true:false)
	{

	}

	FFileMemory::FFileMemory(uint32 nSize)
		: m_pBuffer(new uint8[nSize])
		, m_nSize(nSize)
		, m_nEdge(nSize)
		, m_nPosition(0)
		, m_bFreeOnClose(true)
	{
		
	}

	FFileMemory::~FFileMemory()
	{	
		Close();
	}

	bool FFileMemory::Open()
	{
		if (m_pBuffer) 
			return false;

		m_nPosition		= m_nSize = m_nEdge = 0;
		m_pBuffer		= new uint8[1];
		m_bFreeOnClose	= true;

		return true;
	}

	uint8* FFileMemory::GetBuffer()
	{
		return m_pBuffer;
	}

	uint8* FFileMemory::GetBuffer(bool bDetachBuffer)
	{
		m_bFreeOnClose = !bDetachBuffer;
		return m_pBuffer;
	}

	bool FFileMemory::Close()
	{
		if (m_pBuffer && m_bFreeOnClose)
		{
			F_DELETE_T(m_pBuffer)
			m_nSize = 0;
		}
		return true;
	}

	size_t FFileMemory::Read(void* buffer, size_t size, size_t count)
	{
		if (!buffer || !m_pBuffer) 
			return 0;

		if (m_nPosition >= (long)m_nSize) 
			return 0;

		long nCount = (long)(count*size);
		if (nCount == 0) 
			return 0;

		long nRead;
		if (m_nPosition + nCount > (long)m_nSize)
			nRead = (m_nSize - m_nPosition);
		else
			nRead = nCount;

		memcpy(buffer, m_pBuffer + m_nPosition,nRead);
		m_nPosition += nRead;

		return (size_t)(nRead/size);
	}

	size_t FFileMemory::Write(const void* buffer, size_t size, size_t count)
	{
		if (!buffer || !m_pBuffer) 
			return 0;
		
		long nCount = (long)(count*size);
		if (nCount == 0) 
			return 0;

		if (m_nPosition + nCount > m_nEdge)
			_alloc(m_nPosition + nCount);
		memcpy(m_pBuffer + m_nPosition, buffer, nCount);
		m_nPosition += nCount;

		if (m_nPosition > (long)m_nSize) 
			m_nSize = m_nPosition;

		return count;
	}

	bool FFileMemory::Seek(long offset, int origin)
	{	
		if (!m_pBuffer) 
			return false;
		long lNewPos = m_nPosition;

		if (origin == SEEK_SET)		 
			lNewPos = offset;
		else if (origin == SEEK_CUR)
			lNewPos += offset;
		else if (origin == SEEK_END) 
			lNewPos = m_nSize + offset;
		else 
			return false;

		if (lNewPos < 0) 
			lNewPos = 0;

		m_nPosition = lNewPos;
		return true;
	}

	long FFileMemory::Tell()
	{
		if (!m_pBuffer) 
			return -1;
		return m_nPosition;
	}

	long FFileMemory::Size()
	{
		if (!m_pBuffer)
			return -1;
		return m_nSize;
	}

	bool FFileMemory::Flush()
	{
		if (!m_pBuffer)
			return false;
		return true;
	}

	bool FFileMemory::Eof()
	{
		if (!m_pBuffer)
			return true;
		return (m_nPosition >= (long)m_nSize);
	}

	long FFileMemory::Error()
	{
		if (!m_pBuffer)
			return -1;
		return (m_nPosition > (long)m_nSize);
	}

	bool FFileMemory::PutC(unsigned char c)
	{
		if (!m_pBuffer)
			return false;
		if (m_nPosition + 1 > m_nEdge) 
			_alloc(m_nPosition + 1);
		memcpy(m_pBuffer + m_nPosition, &c, 1);
		m_nPosition += 1;

		if (m_nPosition > (long)m_nSize) 
			m_nSize = m_nPosition;

		return true;
	}	

	long FFileMemory::GetC()
	{
		if (Eof()) 
			return EOF;
		return *(uint8*)((uint8*)m_pBuffer + m_nPosition++);
	}

	char* FFileMemory::GetS(char *string,int n)
	{
		n--;
		long c,i=0;
		while (i<n)
		{
			c = GetC();
			if (c == EOF) 
				return 0;
			string[i++] = (char)c;
			if (c == '\n') 
				break;
		}
		string[i] = 0;
		return string;
	}

	long FFileMemory::Scanf(const char* format, void* output)
	{
		return 0;
	}

	void FFileMemory::_alloc(uint32 dwNewLen)
	{
		if (dwNewLen > (uint32)m_nEdge)
		{
			uint32 dwNewBufferSize = (uint32)(((dwNewLen>>12)+1)<<12);
			F_DELETE_T(m_pBuffer)
			m_pBuffer = new uint8[dwNewBufferSize];
			m_bFreeOnClose = true;
			m_nEdge = dwNewBufferSize;
		}
	}	

	void FFileMemory::_free()
	{
		Close();
	}
    
}; //LostPeterFoundation