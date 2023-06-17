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

#ifndef _F_FILE_IO_H_
#define _F_FILE_IO_H_

#include "FFileBase.h"

namespace LostPeterFoundation
{
    class LPF_Export FFileIO : public FFileBase
	{
	public:
		FFileIO(FILE* fp = 0);
		~FFileIO();

	public:
	protected:
		FILE*	m_fp;
		String	m_strFilePath;
		bool	m_bCloseFile;

	public:
		LP_FORCEINLINE FILE* GetHandle() { return m_fp; }
		LP_FORCEINLINE const String& GetFilePath() const { return m_strFilePath; }

	public:
		bool			Open(const char* filename, const char* mode);
		virtual bool	Close();
		virtual size_t	Read(void* buffer, size_t size, size_t count);
		virtual size_t	Write(const void* buffer, size_t size, size_t count);
		virtual bool	Seek(long offset, int origin);
		virtual long	Tell();
		virtual long	Size();
		virtual bool	Flush();
		virtual bool	Eof();
		virtual long	Error();
		virtual bool	PutC(unsigned char c);
		virtual long	GetC();
		virtual char*	GetS(char* string, int n);				
		virtual long	Scanf(const char* format, void* output);
		String			ReadString();
		void			WriteString(String str);

		bool 			ReadLine(char* szLineBuffer, size_t dwBufferLength, int* pdwReadLength);
		bool 			WriteLine(const char* szLineBuffer);
	};

}; //LostPeterFoundation

#endif