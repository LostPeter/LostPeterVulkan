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

#ifndef _F_STREAM_DATA_FILE_STREAM_H_
#define _F_STREAM_DATA_FILE_STREAM_H_

#include "FStreamData.h"

namespace LostPeterFoundation
{
	class foundationExport FStreamDataFileStream : public FStreamData
	{
	public:
		FStreamDataFileStream(std::ifstream* pStream, bool bIsFreeOnClose = true);
		FStreamDataFileStream(const String& strName, std::ifstream* pStream, bool bIsFreeOnClose = true);
		FStreamDataFileStream(const String& strName, std::ifstream* pStream, size_t nSize, bool bIsFreeOnClose = true);
		
		FStreamDataFileStream(std::fstream* pStream, bool bIsFreeOnClose = true);						
		FStreamDataFileStream(const String& strName, std::fstream* pStream, bool bIsFreeOnClose = true); 
		FStreamDataFileStream(const String& strName, std::fstream* pStream, size_t nSize, bool bIsFreeOnClose = true);
		virtual ~FStreamDataFileStream();

	public:
	protected:
		std::istream* m_pStream;		
		std::ifstream* m_pFStreamRO;	
		std::fstream* m_pFStream;		
		bool m_bIsFreeOnClose;			

	public:
		virtual size_t Read(void* pBuffer, size_t nCount);
		virtual size_t Write(const void* pBuffer, size_t nCount);
		virtual size_t ReadLine(char* pBuffer, size_t nMaxCount, const String& strDelim = "\n");
		virtual void Skip(long nCount);
		virtual void Seek(size_t pos);
		virtual size_t Tell() const;
		virtual bool Eof() const;
		virtual void Close();

	protected:
		void determineAccess();

	};

}; //LostPeterFoundation

#endif