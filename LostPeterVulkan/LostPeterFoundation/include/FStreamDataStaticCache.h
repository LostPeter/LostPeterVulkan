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

#ifndef _F_STREAM_DATA_STATIC_CACHE_H_
#define _F_STREAM_DATA_STATIC_CACHE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	template <size_t nCacheSize>
	class FStreamDataStaticCache
	{
	public:
		FStreamDataStaticCache()
		{
			m_nValidBytes = 0;
			m_nPos = 0;
		}

	public:
	protected:
		char m_szBuffer[nCacheSize];	
		size_t m_nValidBytes;			
		size_t m_nPos;					

	public:
		size_t CacheData(const void* pBuffer, size_t nCount)
		{
			F_Assert("FStreamDataStaticCache::CacheData: It is assumed that you cache data only after you have read everything !" && Avail() == 0)

			if (nCount < nCacheSize)
			{
				if (nCount + m_nValidBytes <= nCacheSize)
				{
					memcpy(m_szBuffer + m_nValidBytes, pBuffer, nCount);
					m_nValidBytes += nCount;
				}
				else
				{
					size_t begOff = nCount - (nCacheSize - m_nValidBytes);
					memmove(m_szBuffer, m_szBuffer + begOff, m_nValidBytes - begOff);
					memcpy(m_szBuffer + nCacheSize - nCount, pBuffer, nCount);
					m_nValidBytes = nCacheSize;
				}
				m_nPos = m_nValidBytes;
				return nCount;
			}
			else
			{
				memcpy(m_szBuffer, (const char*)pBuffer + nCount - nCacheSize, nCacheSize);
				m_nValidBytes = m_nPos = nCacheSize;
				return nCacheSize;
			}
		}
		
		size_t Read(void* pBuffer, size_t nCount)
		{
			size_t rb = Avail();
			rb = (rb < nCount) ? rb : nCount;
			memcpy(pBuffer, m_szBuffer + m_nPos, rb);
			m_nPos += rb;
			return rb;
		}

		bool Rewind(size_t nCount)
		{
			if (m_nPos < nCount)
			{
				Clear();
				return false;
			}
			else
			{
				m_nPos -= nCount;
				return true;
			}
		}
		
		bool Forward(size_t nCount)
		{
			if (Avail() < nCount)
			{
				Clear();
				return false;
			}
			else
			{
				m_nPos += nCount;
				return true;
			}
		}

		size_t Avail() const
		{
			return m_nValidBytes - m_nPos;
		}

		void Clear()
		{
			m_nValidBytes = 0;
			m_nPos = 0;
		}
	};

}; //LostPeterFoundation

#endif