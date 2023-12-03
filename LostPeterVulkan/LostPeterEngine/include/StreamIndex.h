/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _STREAM_INDEX_H_
#define _STREAM_INDEX_H_

#include "Stream.h"

namespace LostPeterEngine
{
    class engineExport StreamIndex : public Stream
    {
    public:
        StreamIndex(EStreamIndexType eStreamIndex, size_t nIndexNum, 
                    EStreamUsageType eStreamUsage, bool bUseSystemMemory, bool bUseShadowStream);
		virtual ~StreamIndex();
	
	public:
	protected:
		EStreamIndexType m_eStreamIndex;
		size_t m_eStreamIndexTypeSize;
		size_t m_eStreamIndexNum;			
		
	public:
		EStreamIndexType GetStreamIndexType() const { return m_eStreamIndex; }
		size_t GetStreamIndexSize() const { return m_eStreamIndexTypeSize; }
		size_t GetStreamIndexNum() const { return m_eStreamIndexNum; }
    };

}; //LostPeterEngine

#endif