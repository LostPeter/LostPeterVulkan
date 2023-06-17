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

#ifndef _F_FILE_MEMORY_H_
#define _F_FILE_MEMORY_H_

#include "FFileBase.h"

namespace LostPeterFoundation
{
     class LPF_Export FFileMemory : public FFileBase
	{
	public:
		FFileMemory(uint8* pBuffer = 0, uint32 nSize = 0);
		FFileMemory(uint32 nSize);
		~FFileMemory();

	public:
	protected:
		uint8* m_pBuffer;
		uint32 m_nSize;
		long m_nEdge;
		long m_nPosition;			
		bool m_bFreeOnClose;

	public:
		LP_FORCEINLINE void SetFreeOnClose(bool b) { m_bFreeOnClose = b; }

	public:
		bool			Open();
		uint8*			GetBuffer();
		uint8*			GetBuffer(bool bDetachBuffer);
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
		virtual char*	GetS(char *string, int n);
		virtual long	Scanf(const char* format, void* output);

	protected:
		void _alloc(uint32 dwNewLen);
		void _free();
	};

}; //LostPeterFoundation

#endif