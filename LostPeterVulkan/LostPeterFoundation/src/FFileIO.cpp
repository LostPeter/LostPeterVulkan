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

#include "../include/FFileIO.h"

namespace LostPeterFoundation
{
    FFileIO::FFileIO(FILE* fp /*= 0*/)
		: m_fp(fp)
		, m_strFilePath("")
		, m_bCloseFile(fp?true:false)
	{
		
	}

	FFileIO::~FFileIO()
	{
		Close();
	}

	bool FFileIO::Open(const char* filename, const char* mode)
	{
		if (m_fp) 
			return false;	
		m_strFilePath = filename;	

		m_fp = fopen(filename, mode);
		if (!m_fp) 
			return false;

		m_bCloseFile = true;
		return true;
	}

	bool FFileIO::Close()
	{
		int iErr = 0;
		if (m_fp && m_bCloseFile)
		{ 
			iErr = fclose(m_fp);
			m_fp = 0;
		}
		m_strFilePath = "";
		return iErr == 0 ? true : false;
	}

	size_t FFileIO::Read(void* buffer, size_t size, size_t count)
	{
		if (!m_fp) 
			return 0;
		return fread(buffer, size, count, m_fp);
	}

	size_t FFileIO::Write(const void* buffer, size_t size, size_t count)
	{
		if (!m_fp) 
			return 0;
		return fwrite(buffer, size, count, m_fp);
	}

	bool FFileIO::Seek(long offset, int origin)
	{
		if (!m_fp) 
			return false;
		return (fseek(m_fp, offset, origin) == 0) ? true : false;
	}

	long FFileIO::Tell()
	{	
		if (!m_fp) 
			return 0;
		return ftell(m_fp);
	}

	long FFileIO::Size()
	{
		if (!m_fp) 
			return -1;
		long pos,size;
		pos = ftell(m_fp);
		fseek(m_fp, 0, SEEK_END);
		size = ftell(m_fp);
		fseek(m_fp, pos, SEEK_SET);
		return size;
	}

	bool FFileIO::Flush()
	{
		if (!m_fp) 
			return false;
		return (fflush(m_fp) == 0)? true : false;
	}

	bool FFileIO::Eof()
	{
		if (!m_fp) 
			return true;
		return (feof(m_fp) != 0)? true : false;
	}

	long FFileIO::Error()
	{
		if (!m_fp) 
			return -1;
		return ferror(m_fp);
	}

	bool FFileIO::PutC(unsigned char c)
	{
		if (!m_fp) 
			return false;
		return (fputc(c, m_fp) == c)? true : false;
	}

	long FFileIO::GetC()
	{
		if (!m_fp) 
			return EOF;
		return getc(m_fp);
	}

	char* FFileIO::GetS(char* string, int n)				
	{
		if (!m_fp) 
			return 0;
		return fgets(string, n, m_fp);
	}

	long FFileIO::Scanf(const char* format, void* output)
	{
		if (!m_fp) 
			return EOF;
		return fscanf(m_fp, format, output);
	}

	String FFileIO::ReadString()
	{
		String str;
		if (!m_fp) 
			return str;

		char c = (char)getc(m_fp);
		while (c != '\0')
		{
			str.push_back(c);
			c = (char)getc(m_fp);
		}
		return str;
	}

	void FFileIO::WriteString(String str)
	{
		int nSize = (int)str.size();
		if(nSize > 0)
			fwrite(str.c_str(), sizeof(char), nSize, m_fp);
		fputc('\0',m_fp);
	}

	bool FFileIO::ReadLine(char* szLineBuffer, size_t dwBufferLength, int *pdwReadLength)
	{
		if (!m_fp) 
			return 0;
		long lCur = Tell();
		bool bRet = (fgets(szLineBuffer, dwBufferLength, m_fp) != 0); 
		*pdwReadLength = int(Tell() - lCur);
		return bRet;
	}

	bool FFileIO::WriteLine(const char* szLineBuffer)
	{
		if (!m_fp) 
			return 0;

		return fprintf(m_fp, "%s\n", szLineBuffer) >= 0;
	}
    
}; //LostPeterFoundation