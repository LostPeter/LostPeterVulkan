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

#include "../include/FStreamData.h"
#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
    FStreamData::FStreamData(int eStreamAccess /*= F_StreamAccess_Read*/)
        : m_strName("")
        , m_nSize(0)	
		, m_eStreamAccess(eStreamAccess)
    {

    }

    FStreamData::FStreamData(const String& strName, int eStreamAccess /*= F_StreamAccess_Read*/)
        : m_strName(strName)
        , m_nSize(0) 
        , m_eStreamAccess(eStreamAccess)
    {

    }

	FStreamData::~FStreamData()
    {
        
    }

    template <typename T> 
    FStreamData& FStreamData::operator >>(T& val)
	{
		Read(static_cast<void*>(&val), sizeof(T));
		return *this;
	}

	size_t FStreamData::ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim)
	{
		bool trimCR = false;
		if (strDelim.find_first_of('\n') != String::npos)
		{
			trimCR = true;
		}

		char tmpBuf[F_C_STREAM_SIZE_TEMP];
		size_t chunkSize = F_M_MIN(nMaxCount, (size_t)F_C_STREAM_SIZE_TEMP-1);
		size_t totalCount = 0;
		size_t readCount; 
		while (chunkSize && (readCount = Read(tmpBuf, chunkSize)))
		{
			tmpBuf[readCount] = '\0';

			size_t pos = strcspn(tmpBuf, strDelim.c_str());

			if (pos < readCount)
			{
				Skip((long)(pos + 1 - readCount));
			}

			if (pBuffer)
			{
				memcpy(pBuffer+totalCount, tmpBuf, pos);
			}
			totalCount += pos;

			if (pos < readCount)
			{
				if (trimCR && totalCount && pBuffer[totalCount-1] == '\r')
				{
					--totalCount;
				}

				break;
			}

			chunkSize = F_M_MIN(nMaxCount-totalCount, (size_t)F_C_STREAM_SIZE_TEMP - 1);
		}

		pBuffer[totalCount] = '\0';
		return totalCount;
	}

	String FStreamData::GetLine(bool bIsTrimAfter)
	{
		char tmpBuf[F_C_STREAM_SIZE_TEMP];
		String retString;
		size_t readCount;

		while ((readCount = Read(tmpBuf, F_C_STREAM_SIZE_TEMP - 1)) != 0)
		{
			tmpBuf[readCount] = '\0';

			char* p = strchr(tmpBuf, '\n');
			if (p != 0)
			{
				Skip((long)(p + 1 - tmpBuf - readCount));
				*p = '\0';
			}
			retString += tmpBuf;

			if (p != 0)
			{
				if (retString.length() && retString[retString.length()-1] == '\r')
				{
					retString.erase(retString.length()-1, 1);
				}

				break;
			}
		}

		if (bIsTrimAfter)
		{
			FUtilString::Trim(retString);
		}

		return retString;
	}

	String FStreamData::GetAsString()
	{
        String str = "";
		char* pBuf = new char[m_nSize + 1];
        {
            Seek(0);
            Read(pBuf, m_nSize);
            pBuf[m_nSize] = '\0';
            str.insert(0, pBuf, m_nSize);
        }
		delete []pBuf;
		return str;
	}

	size_t FStreamData::SkipLine(const String& strDelim)
	{
		char tmpBuf[F_C_STREAM_SIZE_TEMP];
		size_t total = 0;
		size_t readCount;

		while ((readCount = Read(tmpBuf, F_C_STREAM_SIZE_TEMP-1)) != 0)
		{
			tmpBuf[readCount] = '\0';
			size_t pos = strcspn(tmpBuf, strDelim.c_str());

			if (pos < readCount)
			{
				Skip((long)(pos + 1 - readCount));
				total += pos + 1;

				break;
			}

			total += readCount;
		}

		return total;
	}

}; //LostPeterFoundation