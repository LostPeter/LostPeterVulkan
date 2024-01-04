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

#include "../include/FStreamDataFileStream.h"

namespace LostPeterFoundation
{
    FStreamDataFileStream::FStreamDataFileStream(std::ifstream* pStream, bool bIsFreeOnClose)
		: FStreamData()
		, m_pStream(pStream)
		, m_pFStreamRO(pStream)
		, m_pFStream(nullptr)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_pStream->seekg(0, std::ios_base::end);
		m_nSize = m_pStream->tellg();
		m_pStream->seekg(0, std::ios_base::beg);
		determineAccess();
	}

	FStreamDataFileStream::FStreamDataFileStream(const String& strName, std::ifstream* pStream, bool bIsFreeOnClose)
		: FStreamData(strName)
		, m_pStream(pStream)
		, m_pFStreamRO(pStream)
		, m_pFStream(nullptr)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_pStream->seekg(0, std::ios_base::end);
		m_nSize = m_pStream->tellg();
		m_pStream->seekg(0, std::ios_base::beg);
		determineAccess();
	}

	FStreamDataFileStream::FStreamDataFileStream(const String& strName, std::ifstream* pStream, size_t nSize, bool bIsFreeOnClose)
		: FStreamData(strName)
		, m_pStream(pStream)
		, m_pFStreamRO(pStream)
		, m_pFStream(nullptr)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_nSize = nSize;
		determineAccess();
	}

	FStreamDataFileStream::FStreamDataFileStream(std::fstream* pStream, bool bIsFreeOnClose)
		: FStreamData(false)
		, m_pStream(pStream)
		, m_pFStreamRO(nullptr)
		, m_pFStream(pStream)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_pStream->seekg(0, std::ios_base::end);
		m_nSize = (size_t)m_pStream->tellg();
		m_pStream->seekg(0, std::ios_base::beg);
		determineAccess();
	}

	FStreamDataFileStream::FStreamDataFileStream(const String& strName, std::fstream* pStream, bool bIsFreeOnClose)
		: FStreamData(strName,false)
		, m_pStream(pStream)
		, m_pFStreamRO(nullptr)
		, m_pFStream(pStream)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_pStream->seekg(0, std::ios_base::end);
		m_nSize = (size_t)m_pStream->tellg();
		m_pStream->seekg(0, std::ios_base::beg);
		determineAccess();
	}

	FStreamDataFileStream::FStreamDataFileStream(const String& strName, std::fstream* pStream, size_t nSize, bool bIsFreeOnClose)
		: FStreamData(strName,false)
		, m_pStream(pStream)
		, m_pFStreamRO(nullptr)
		, m_pFStream(pStream)
		, m_bIsFreeOnClose(bIsFreeOnClose)
	{
		m_nSize = nSize;
		determineAccess();
	}

	FStreamDataFileStream::~FStreamDataFileStream()
	{
		Close();
	}

	void FStreamDataFileStream::determineAccess()
	{
		m_eStreamAccess = 0;
		if (m_pStream)
			m_eStreamAccess |= F_StreamAccess_Read;
		if (m_pFStream)
			m_eStreamAccess |= F_StreamAccess_Write;
	}

	size_t FStreamDataFileStream::Read(void* pBuffer, size_t nCount)
	{
		m_pStream->read(static_cast<char*>(pBuffer), static_cast<std::streamsize>(nCount));
		return m_pStream->gcount();
	}

	size_t FStreamDataFileStream::Write(const void* pBuffer, size_t nCount)
	{
		size_t written = 0;
		if (IsWriteable() && m_pFStream)
		{
			m_pFStream->write(static_cast<const char*>(pBuffer), static_cast<std::streamsize>(nCount));
			written = nCount;
		}
		return written;
	}

	size_t FStreamDataFileStream::ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim)
	{
		if (strDelim.empty())
		{
            F_LogError("*********************** FStreamDataFileStream::ReadLine: No delimiter provided !");
			return 0;
		}
		if (strDelim.size() > 1)
		{
            F_LogWarning("*********************** FStreamDataFileStream::ReadLine: Delimiter size must <= 1 !");
		}

		bool trimCR = false;
		if (strDelim.at(0) == '\n') 
		{
			trimCR = true;
		}
		m_pStream->getline(pBuffer, static_cast<std::streamsize>(nMaxCount + 1), strDelim.at(0));
		size_t ret = m_pStream->gcount();

		if (m_pStream->eof()) 
		{

		}
		else if (m_pStream->fail())
		{
			if (ret == nMaxCount)
			{
				m_pStream->clear();
			}
			else
			{
                F_LogError("*********************** FStreamDataFileStream::ReadLine: Streaming error occurred !");
			    return 0;
			}
		}
		else 
		{
			--ret;
		}

		if (trimCR && pBuffer[ret-1] == '\r')
		{
			--ret;
			pBuffer[ret] = '\0';
		}
		return ret;
	}

	void FStreamDataFileStream::Skip(long nCount)
	{
#if defined(STLPORT)
		if (m_pStream->eof())
		{
			m_pStream->clear();
			m_pStream->seekg(0, std::ios::end);
		}
#endif 		

		m_pStream->clear();
		m_pStream->seekg(static_cast<std::ifstream::pos_type>(nCount), std::ios::cur);
	}

	void FStreamDataFileStream::Seek(size_t pos)
	{
		m_pStream->clear(); 
		m_pStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
	}

	size_t FStreamDataFileStream::Tell(void) const
	{
		m_pStream->clear();
		return m_pStream->tellg();
	}

	bool FStreamDataFileStream::Eof(void) const
	{
		return m_pStream->eof();
	}

	void FStreamDataFileStream::Close(void)
	{
		if (m_pStream)
		{
			if (m_pFStreamRO)
				m_pFStreamRO->close();

			if (m_pFStream)
			{
				m_pFStream->flush();
				m_pFStream->close();
			}

			if (m_bIsFreeOnClose)
			{
				if (m_pFStreamRO)
					delete m_pFStreamRO;
				if (m_pFStream)
					delete m_pFStream;
				m_pStream = nullptr;
				m_pFStreamRO = nullptr; 
				m_pFStream = nullptr; 
			}
		}
	}

}; //LostPeterFoundation