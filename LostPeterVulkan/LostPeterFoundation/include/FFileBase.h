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

#ifndef _F_FILE_BASE_H_
#define _F_FILE_BASE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FFileBase
	{
	public:
		FFileBase()
		{

		}
		virtual ~FFileBase()
		{

		}

	public:
		virtual bool	Close()												= 0;						
		virtual size_t	Read(void* buffer, size_t size, size_t count)		= 0;	
		virtual size_t	Write(const void* buffer, size_t size, size_t count)= 0;	
		virtual bool	Seek(long offset,int origin)						= 0;	
		virtual long	Tell()												= 0;	
		virtual long	Size()												= 0;	
		virtual bool	Flush()												= 0;	
		virtual bool	Eof()												= 0;	
		virtual long	Error()												= 0;	
		virtual bool	PutC(unsigned char c);										
		virtual long	GetC()												= 0;	
		virtual char*	GetS(char* string, int n)							= 0;	
		virtual long	Scanf(const char* format, void* output)				= 0;	
	};

}; //LostPeterFoundation

#endif