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

#ifndef _F_STREAM_DATA_DEFLATE_H_
#define _F_STREAM_DATA_DEFLATE_H_

#include "FStreamData.h"
#include "FStreamDataStaticCache.h"

struct z_stream_s;
typedef struct z_stream_s z_stream;

namespace LostPeterFoundation
{
	class foundationExport FStreamDataDeflate : public FStreamData
	{
	public:
		FStreamDataDeflate(FStreamData* pStreamCompressed);
		FStreamDataDeflate(const String& strName, FStreamData* pStreamCompressed);	
		virtual ~FStreamDataDeflate();

	public:
	protected:
		FStreamData* m_pStreamCompressed;	
		FStreamData* m_pStreamWriteTemp;		
		String m_strTempFileName;		
		z_stream* m_pStreamZip;				
		size_t m_nCurrentPos;			
		FStreamDataStaticCache<16 * F_C_STREAM_SIZE_TEMP> m_ReadCache;	
		uint8* m_pTemp;					
		bool m_bIsCompressedValid;	

	public:
		F_FORCEINLINE bool IsCompressedStreamValid() const { return m_bIsCompressedValid; }

	public:
		virtual size_t Read(void* pBuffer, size_t nCount);
		virtual size_t Write(const void* pBuffer, size_t nCount);
		virtual void Skip(long nCount);
		virtual void Seek(size_t nPos);
		virtual size_t Tell() const;
		virtual bool Eof() const;
		virtual void Close();

	protected:
		void destroy();
		void init();
		
		void compressFinal();
	};

}; //LostPeterFoundation

#endif