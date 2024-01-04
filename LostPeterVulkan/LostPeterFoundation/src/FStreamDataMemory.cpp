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

#include "../include/FStreamDataMemory.h"

namespace LostPeterFoundation
{
    FStreamDataMemory::FStreamDataMemory(void* pMemory, size_t nSize, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_pData	= m_pPos = static_cast<uint8*>(pMemory);
		m_nSize	= nSize;
		m_pEnd = m_pData + m_nSize;
		m_bIsFreeOnClose = bIsFreeOnClose;
		F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(const String& strName, void* pMemory, size_t nSize, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(strName,static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_pData	= m_pPos = static_cast<uint8*>(pMemory);
		m_nSize	= nSize;
		m_pEnd = m_pData + m_nSize;
		m_bIsFreeOnClose = bIsFreeOnClose;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(FStreamData& streamSrc, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= streamSrc.Size();
		m_pData	= new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + streamSrc.Read(m_pData, m_nSize);
		m_bIsFreeOnClose = bIsFreeOnClose;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(FStreamData* pStreamSrc, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= pStreamSrc->Size();
		m_pData	= new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + pStreamSrc->Read(m_pData, m_nSize);
		m_bIsFreeOnClose = bIsFreeOnClose;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(const String& strName, FStreamData& streamSrc, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(strName,static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= streamSrc.Size();
		m_pData	= new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + streamSrc.Read(m_pData, m_nSize);
		m_bIsFreeOnClose = bIsFreeOnClose;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(const String& strName, FStreamData* pStreamSrc, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(strName,static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= pStreamSrc->Size();
		m_pData	= new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + pStreamSrc->Read(m_pData, m_nSize);
		m_bIsFreeOnClose = bIsFreeOnClose;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(size_t nSize, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= nSize;
		m_bIsFreeOnClose = bIsFreeOnClose;
		m_pData = new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + m_nSize;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::FStreamDataMemory(const String& strName, size_t nSize, bool bIsFreeOnClose, bool bIsReadOnly)
		: FStreamData(strName,static_cast<int>(bIsReadOnly ? F_StreamAccess_Read : (F_StreamAccess_Read | F_StreamAccess_Write)))
	{
		m_nSize	= nSize;
		m_bIsFreeOnClose = bIsFreeOnClose;
		m_pData	= new uint8[m_nSize];
		m_pPos = m_pData;
		m_pEnd = m_pData + m_nSize;
        F_Assert("FStreamDataMemory::FStreamDataMemory" && m_pEnd >= m_pPos)
	}

	FStreamDataMemory::~FStreamDataMemory()
	{
		Close();
	}

	size_t FStreamDataMemory::Read(void* pBuffer, size_t nCount)
	{
		size_t cnt = nCount;
		if (m_pPos + cnt > m_pEnd)
			cnt = m_pEnd - m_pPos;
		if (cnt == 0)
			return 0;
		F_Assert("FStreamDataMemory::Read" && cnt <= nCount)

		memcpy(pBuffer, m_pPos, cnt);
		m_pPos += cnt;
		return cnt;
	}

	size_t FStreamDataMemory::Write(const void* pBuffer, size_t nCount)
	{
		size_t written = 0;
		if (IsWriteable())
		{
			written = nCount;
			if (m_pPos + written > m_pEnd)
				written = m_pEnd - m_pPos;
			if (written == 0)
				return 0;

			memcpy(m_pPos,pBuffer,written);
			m_pPos += written;
		}
		return written;
	}

	size_t FStreamDataMemory::ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim)
	{
		bool trimCR = false;
		if (strDelim.find_first_of('\n') != String::npos)
		{
			trimCR = true;
		}

		size_t nPos = 0;
		while (nPos < nMaxCount && m_pPos < m_pEnd)
		{
			if (strDelim.find(*m_pPos) != String::npos)
			{
				if (trimCR && nPos && pBuffer[nPos - 1] == '\r')
				{
					--nPos;
				}

				++m_pPos;
				break;
			}

			pBuffer[nPos++] = *m_pPos++;
		}

		pBuffer[nPos] = '\0';
		return nPos;
	}

	size_t FStreamDataMemory::SkipLine(const String& strDelim)
	{
		size_t nPos = 0;
		while (m_pPos < m_pEnd)
		{
			++nPos;
			if (strDelim.find(*m_pPos++) != String::npos)
			{
				break;
			}
		}
		return nPos;
	}

	void FStreamDataMemory::Skip(long nCount)
	{
		size_t newpos = (size_t)((m_pPos - m_pData) + nCount);
        F_Assert("FStreamDataMemory::Skip" && m_pData + newpos <= m_pEnd)  

		m_pPos = m_pData + newpos;
	}

	void FStreamDataMemory::Seek(size_t nPos)
	{
        F_Assert("FStreamDataMemory::Seek" && m_pData + nPos <= m_pEnd)  
		m_pPos = m_pData + nPos;
	}

	size_t FStreamDataMemory::Tell() const
	{
		return m_pPos - m_pData;
	}

	bool FStreamDataMemory::Eof() const
	{
		return m_pPos >= m_pEnd;
	}

	void FStreamDataMemory::Close()    
	{
		if (m_bIsFreeOnClose && m_pData)
		{
			delete []m_pData;
			m_pData = nullptr;
		}
	}

}; //LostPeterFoundation